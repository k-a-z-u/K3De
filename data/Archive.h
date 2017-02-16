#ifndef ARCHIVE_H
#define ARCHIVE_H

extern "C" {
	#include "../lib/7z/7z.h"
	#include "../lib/7z/7zAlloc.h"
	#include "../lib/7z/7zFile.h"
	#include "../lib/7z/7zCrc.h"
	#include "../lib/7z/7zVersion.h"
}

#include "Data.h"
#include "../Exception.h"

#include <unordered_map>
#include <mutex>

class Archive {

private:

	CFileInStream archiveStream;
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;
	CLookToRead lookStream;
	CSzArEx db;
	std::mutex mtx;

	/** filename -> handle. for all files in the archive */
	std::unordered_map<std::string, UInt32> files;

public:

	/** dtor */
	~Archive() {

		// cleanup
		SzArEx_Free(&db, &allocImp);
		File_Close(&archiveStream.file);

	}

	/** ctor */
	Archive(const std::string& fileName) {

		static constexpr int MAX_NAME_LEN = 4096;

		SRes res;
		UInt16 tmpName[MAX_NAME_LEN];

		allocImp.Alloc = SzAlloc;
		allocImp.Free = SzFree;

		allocTempImp.Alloc = SzAllocTemp;
		allocTempImp.Free = SzFreeTemp;

		if (InFile_Open(&archiveStream.file, fileName.c_str())) {
			throw Exception("can not open input file: " + fileName);
		}

		FileInStream_CreateVTable(&archiveStream);
		LookToRead_CreateVTable(&lookStream, False);

		lookStream.realStream = &archiveStream.s;
		LookToRead_Init(&lookStream);

		CrcGenerateTable();

		SzArEx_Init(&db);

		// open the archive
		res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
		if (res != SZ_OK) {throw Exception("error while opening archive");}

		// iterate over all files
		for (UInt32 i = 0; i < db.NumFiles; i++){

			const unsigned isDir = SzArEx_IsDir(&db, i);
			if (isDir) {continue;}

			// fetch the file's name as utf16
			size_t nameLen = SzArEx_GetFileNameUtf16(&db, i, NULL);
			if (nameLen > MAX_NAME_LEN) {throw Exception("invalid file name length");}
			SzArEx_GetFileNameUtf16(&db, i, tmpName);

			// debug
			files[toString(tmpName)] = i;
			std::cout << i << ": " << toString(tmpName) << std::endl;

		}

	}

	/** extract the file named "file". throws exception if file does not exist */
	Data get(const std::string& file) {
		auto it = files.find(file);
		if (it == files.end()) {throw Exception("file not found: " + file);}
		const UInt32 id = it->second;
		return extract(id);
	}

	/** does the archive contain a file named "file" ? */
	bool hasFile(const std::string& file) const {
		auto it = files.find(file);
		return it != files.end();
	}


private:

	Data extract(const UInt32 idx) {

		mtx.lock();

		UInt32 blockIndex = 0xFFFFFFFF;		/* it can have any value before first call (if outBuffer = 0) */
		Byte* outBuffer = 0;				/* it must be 0 before first call for each new archive. */
		size_t outBufferSize = 0;
		size_t offset = 0;
		size_t outSizeProcessed = 0;

		// decompress completely
		SRes res = SzArEx_Extract(&db, &lookStream.s, idx,
								  &blockIndex, &outBuffer, &outBufferSize,
								  &offset, &outSizeProcessed,
								  &allocImp, &allocTempImp);

		// check result
		if (res != SZ_OK) {throw Exception("error while decompressing file");}

		// pointer to decompressed data and length
		uint8_t* data = outBuffer + offset;
		size_t len = outSizeProcessed;

		// copy to output
		Data out = Data::alloc(len);
		memcpy(out.get(), data, len);

		// cleanup
		IAlloc_Free(&allocImp, outBuffer);

		mtx.unlock();

		return out;

	}





private:

	/** convert wchar_t to char_t */
	static std::string toString(const UInt16* s) {
		std::string ret = "";
		while (*s) {
			ret += (uint8_t) *s;
			++s;
		}
		return ret;
	}


};

#endif // ARCHIVE_H
