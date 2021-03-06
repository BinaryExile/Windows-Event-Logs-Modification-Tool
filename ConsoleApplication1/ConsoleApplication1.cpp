// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "evtx_file_header.h"
#include "evtx_chunk.h"
#include <fstream>
#include <sys/stat.h>
#include "evtx_event_record.h"
#include "crc.h"

void readfileheader(std::ifstream& inFile, evtx_file_header* fileheaderref)
{
	evtx_file_header fileheader = *fileheaderref;
	inFile.read((char *)fileheader.signature, sizeof(fileheader.signature));
	inFile.read((char *)fileheader.first_chunk_number, sizeof(fileheader.first_chunk_number));
	inFile.read((char *)fileheader.last_chunk_number, sizeof(fileheader.last_chunk_number));
	inFile.read((char *)fileheader.next_record_identifier, sizeof(fileheader.next_record_identifier));
	inFile.read((char *)fileheader.header_size, sizeof(fileheader.header_size));
	inFile.read((char *)fileheader.minor_version, sizeof(fileheader.minor_version));
	inFile.read((char *)fileheader.major_version, sizeof(fileheader.major_version));
	inFile.read((char *)fileheader.header_block_size, sizeof(fileheader.header_block_size));
	inFile.read((char *)fileheader.number_of_chunks, sizeof(fileheader.number_of_chunks));
	inFile.read((char *)fileheader.unknown1, sizeof(fileheader.unknown1));
	inFile.read((char *)fileheader.file_flags, sizeof(fileheader.file_flags));
	inFile.read((char *)fileheader.checksum, sizeof(fileheader.checksum));
	inFile.read((char *)fileheader.unknown2, sizeof(fileheader.unknown2));
	*fileheaderref = fileheader;
}

void writefileheader(std::fstream& outFile, evtx_file_header* fileheaderref)
{
	evtx_file_header fileheader = *fileheaderref;
	outFile.write((char *)fileheader.signature, sizeof(fileheader.signature));
	outFile.write((char *)fileheader.first_chunk_number, sizeof(fileheader.first_chunk_number));
	outFile.write((char *)fileheader.last_chunk_number, sizeof(fileheader.last_chunk_number));
	outFile.write((char *)fileheader.next_record_identifier, sizeof(fileheader.next_record_identifier));
	outFile.write((char *)fileheader.header_size, sizeof(fileheader.header_size));
	outFile.write((char *)fileheader.minor_version, sizeof(fileheader.minor_version));
	outFile.write((char *)fileheader.major_version, sizeof(fileheader.major_version));
	outFile.write((char *)fileheader.header_block_size, sizeof(fileheader.header_block_size));
	outFile.write((char *)fileheader.number_of_chunks, sizeof(fileheader.number_of_chunks));
	outFile.write((char *)fileheader.unknown1, sizeof(fileheader.unknown1));
	outFile.write((char *)fileheader.file_flags, sizeof(fileheader.file_flags));
	outFile.write((char *)fileheader.checksum, sizeof(fileheader.checksum));
	outFile.write((char *)fileheader.unknown2, sizeof(fileheader.unknown2));
}

void readchunk(std::ifstream& inFile, evtx_chunk_header* chunkheaderref, std::streampos* location, std::streampos* event_128_512, std::streampos* event_checksumHeader, std::streampos* event_lasteventnum, std::streampos* event_lasteventid)
{
	evtx_chunk_header chunkheader = * chunkheaderref;
	inFile.read((char *)chunkheader.signature, sizeof(chunkheader.signature));
	inFile.read((char *)chunkheader.first_event_record_number, sizeof(chunkheader.first_event_record_number));
	*event_lasteventnum = inFile.tellg();
	inFile.read((char *)chunkheader.last_event_record_number, sizeof(chunkheader.last_event_record_number));
	inFile.read((char *)chunkheader.first_event_record_identifier, sizeof(chunkheader.first_event_record_identifier));
	*event_lasteventid = inFile.tellg();
	inFile.read((char *)chunkheader.last_event_record_identifier, sizeof(chunkheader.last_event_record_identifier));
	inFile.read((char *)chunkheader.header_size, sizeof(chunkheader.header_size));
	
	inFile.read((char *)chunkheader.last_event_record_offset, sizeof(chunkheader.last_event_record_offset));
	inFile.read((char *)chunkheader.free_space_offset, sizeof(chunkheader.free_space_offset));
	*location = inFile.tellg();
	inFile.read((char *)chunkheader.event_records_checksum, sizeof(chunkheader.event_records_checksum));
	inFile.read((char *)chunkheader.unknown1, sizeof(chunkheader.unknown1));
	inFile.read((char *)chunkheader.unknown2, sizeof(chunkheader.unknown2));
	*event_checksumHeader = inFile.tellg();
	inFile.read((char *)chunkheader.checksum, sizeof(chunkheader.checksum));
	*event_128_512 = inFile.tellg();
	inFile.read((char *)chunkheader.common_strings, sizeof(chunkheader.common_strings));
	inFile.read((char *)chunkheader.templateptr, sizeof(chunkheader.templateptr));
	*chunkheaderref = chunkheader;
}

void writechunk(std::fstream& outFile, evtx_chunk_header* chunkheaderref)
{
	evtx_chunk_header chunkheader = *chunkheaderref;
	outFile.write((char *)chunkheader.signature, sizeof(chunkheader.signature));
	outFile.write((char *)chunkheader.first_event_record_number, sizeof(chunkheader.first_event_record_number));
	outFile.write((char *)chunkheader.last_event_record_number, sizeof(chunkheader.last_event_record_number));
	outFile.write((char *)chunkheader.first_event_record_identifier, sizeof(chunkheader.first_event_record_identifier));
	outFile.write((char *)chunkheader.last_event_record_identifier, sizeof(chunkheader.last_event_record_identifier));
	outFile.write((char *)chunkheader.header_size, sizeof(chunkheader.header_size));
	outFile.write((char *)chunkheader.last_event_record_offset, sizeof(chunkheader.last_event_record_offset));
	outFile.write((char *)chunkheader.free_space_offset, sizeof(chunkheader.free_space_offset));
	outFile.write((char *)chunkheader.event_records_checksum, sizeof(chunkheader.event_records_checksum));
	outFile.write((char *)chunkheader.unknown1, sizeof(chunkheader.unknown1));
	outFile.write((char *)chunkheader.unknown2, sizeof(chunkheader.unknown2));
	outFile.write((char *)chunkheader.checksum, sizeof(chunkheader.checksum));
	outFile.write((char *)chunkheader.common_strings, sizeof(chunkheader.common_strings));
	outFile.write((char *)chunkheader.templateptr, sizeof(chunkheader.templateptr));
}

int readsignature(std::ifstream& inFile, evtx_event_record_header* recordheaderref)
{
	evtx_event_record_header recordheader = *recordheaderref;
	inFile.read((char *)recordheader.signature, sizeof(recordheader.signature));
	*recordheaderref = recordheader;
	int sig = recordheader.signature[0];
	sig += (recordheader.signature[1] << 8);
	return sig;
}

void writesignature(std::fstream& outFile, evtx_event_record_header* recordheaderref)
{
	evtx_event_record_header recordheader = *recordheaderref;
	outFile.write((char *)recordheader.signature, sizeof(recordheader.signature));
}

void readrecordheader(std::ifstream& inFile, evtx_event_record_header* recordheaderref)
{
	evtx_event_record_header recordheader = *recordheaderref;
	recordheader.size_location = inFile.tellg();
	inFile.read((char *)recordheader.size, sizeof(recordheader.size));
	unsigned int recordsize = (unsigned int)recordheader.size[0];
	recordsize += (unsigned int)(recordheader.size[1] << 8);
	recordsize -= 28;
	recordheader.recordsize = recordsize;
	recordheader.identifier_location = inFile.tellg();
	inFile.read((char *)recordheader.identifier, sizeof(recordheader.identifier));
	inFile.read((char *)recordheader.written_time, sizeof(recordheader.written_time));
	recordheader.event = new uint8_t[recordsize];
	inFile.read((char *)recordheader.event, recordsize);
	recordheader.size2_location = inFile.tellg();
	inFile.read((char*)recordheader.size2, sizeof(recordheader.size2));
	*recordheaderref = recordheader;
}

void writerecordheader(std::fstream& outFile, evtx_event_record_header* recordheaderref)
{
	evtx_event_record_header recordheader = *recordheaderref;
	outFile.write((char *)recordheader.size, sizeof(recordheader.size));
	outFile.write((char *)recordheader.identifier, sizeof(recordheader.identifier));
	outFile.write((char *)recordheader.written_time, sizeof(recordheader.written_time));
	outFile.write((char *)recordheader.event, recordheader.recordsize);
	outFile.write((char*)recordheader.size2, sizeof(recordheader.size2));
}

int eventchecksum(std::fstream& inFile, std::streampos rec_start_location, std::streampos rec_end_location)
{
	std::streampos fsize = rec_start_location;
	inFile.seekg(rec_end_location);
	std::streampos temp = inFile.tellg();
	std::streampos remsize = rec_end_location - rec_start_location;
	unsigned int remsize_int = (unsigned int)remsize;
	inFile.seekg(fsize);
	uint8_t* remains = new uint8_t[remsize];
	inFile.read((char *)remains, remsize_int);
	int crcval = CRC::Calculate((char *)remains, remsize_int, CRC::CRC_32());
	return crcval;
}

bool ModifyLog(std::ifstream& inFile, std::fstream& outFile, uint8_t *recordtodelete)
{
	std::uint32_t crcval = 0;
	std::streampos rec_end_location;
	std::streampos rec_start_location;
	std::streampos event_crc_location;
	evtx_file_header fileheader;
	evtx_chunk_header chunkheader;
	evtx_event_record_header recordheader;
	std::streampos prev_size_location;
	std::streampos prev_size2_location;
	std::streampos cur_size2_location;
	std::streampos event_first120;
	std::streampos event_128_512;
	std::streampos event_checksumHeader;
	std::streampos event_lasteventnum;
	std::streampos event_lasteventid;
	uint8_t checksumheaderdata[504];
	uint8_t prevsize[4];
	uint8_t newsize[4];
	bool renumber = false;
	INT64 lasteventID;
	INT64 lasteventnum;

	//read data
	readfileheader(inFile, &fileheader);
	writefileheader(outFile, &fileheader);
	event_first120 = inFile.tellg();
	readchunk(inFile, &chunkheader, &event_crc_location, &event_128_512, &event_checksumHeader, &event_lasteventnum, &event_lasteventid);
	lasteventID = chunkheader.last_event_record_identifier[0];
	lasteventID += (chunkheader.last_event_record_identifier[1] << 8);
	lasteventID += (chunkheader.last_event_record_identifier[2] << 16);
	lasteventID += (chunkheader.last_event_record_identifier[3] << 24);
	lasteventID += (chunkheader.last_event_record_identifier[4] << 32);
	lasteventID += (chunkheader.last_event_record_identifier[5] << 40);
	lasteventID += (chunkheader.last_event_record_identifier[6] << 48);
	lasteventID += (chunkheader.last_event_record_identifier[7] << 56);
	lasteventID -= 1;
	lasteventnum = chunkheader.last_event_record_number[0];
	lasteventnum += (chunkheader.last_event_record_number[1] << 8);
	lasteventnum += (chunkheader.last_event_record_number[2] << 16);
	lasteventnum += (chunkheader.last_event_record_number[3] << 24);
	lasteventnum += (chunkheader.last_event_record_number[4] << 32);
	lasteventnum += (chunkheader.last_event_record_number[5] << 40);
	lasteventnum += (chunkheader.last_event_record_number[6] << 48);
	lasteventnum += (chunkheader.last_event_record_number[7] << 56);
	lasteventnum -= 1;
	writechunk(outFile, &chunkheader);
	rec_start_location = inFile.tellg();
	int temp = readsignature(inFile, &recordheader);
	writesignature(outFile, &recordheader);
	//crcval = CRC::Calculate((char *)recordheader.signature, sizeof(recordheader.signature), CRC::CRC_32());


	//while there are still records
	while (temp == 0x2a2a)
	{
		readrecordheader(inFile, &recordheader);
		if (renumber)
		{
			INT64 newsizeINT = recordheader.identifier[0];
			newsizeINT += (recordheader.identifier[1] << 8);
			newsizeINT += (recordheader.identifier[2] << 16);
			newsizeINT += (recordheader.identifier[3] << 24);
			newsizeINT += (recordheader.identifier[4] << 32);
			newsizeINT += (recordheader.identifier[5] << 40);
			newsizeINT += (recordheader.identifier[6] << 48);
			newsizeINT += (recordheader.identifier[7] << 56);
			newsizeINT -= 1;
			recordheader.identifier[0] = newsizeINT & 0x00000000000000ff;
			recordheader.identifier[1] = (newsizeINT & 0x000000000000ff00) >> 8;
			recordheader.identifier[2] = (newsizeINT & 0x0000000000ff0000) >> 16;
			recordheader.identifier[3] = (newsizeINT & 0x00000000ff000000) >> 24;
			recordheader.identifier[4] = (newsizeINT & 0x000000ff00000000) >> 32;
			recordheader.identifier[5] = (newsizeINT & 0x0000ff0000000000) >> 40;
			recordheader.identifier[6] = (newsizeINT & 0x00ff000000000000) >> 48;
			recordheader.identifier[7] = (newsizeINT & 0xff00000000000000) >> 56;


		}
		cur_size2_location = recordheader.size2_location;
		writerecordheader(outFile, &recordheader);

		//When target record is identified
		if ((*recordheader.identifier == *recordtodelete) && renumber == false)
		{
			std::streampos currPos= outFile.tellp();
			std::flush(outFile);
			INT64 prevsizeINT = prevsize[0];
			prevsizeINT += (prevsize[1] << 8);
			prevsizeINT += (prevsize[2] << 16);
			prevsizeINT += (prevsize[3] << 24);
			INT64 sizeINT = recordheader.size[0];
			sizeINT += (recordheader.size[1] << 8);
			sizeINT += (recordheader.size[2] << 16);
			sizeINT += (recordheader.size[3] << 24);
			INT64 newsizeINT = prevsizeINT + sizeINT;
			newsize[0] = newsizeINT & 0x000000ff;
			newsize[1] = (newsizeINT & 0x0000ff00) >> 8;
			newsize[2] = (newsizeINT & 0x00ff0000) >> 16;
			newsize[3] = (newsizeINT & 0xff000000) >> 24;
			printf("\nPrev Size: %i\n Current Size: %i \n NewSize: %i \n", *prevsize, *recordheader.size, newsize);
			outFile.seekp(prev_size_location);
			outFile.write((char *) newsize, sizeof(newsize));
			std::flush(outFile);
			outFile.seekp(cur_size2_location);
			outFile.write((char *)newsize,sizeof(newsize));
			std::flush(outFile);
			outFile.seekp(currPos);
			renumber = true;
		}
	
		prev_size_location = recordheader.size_location;
		prev_size2_location = recordheader.size2_location;
		prevsize[0] = recordheader.size[0];
		prevsize[1] = recordheader.size[1];
		prevsize[2] = recordheader.size[2];
		prevsize[3] = recordheader.size[3];

		rec_end_location = inFile.tellg();
		temp = readsignature(inFile, &recordheader);
		writesignature(outFile, &recordheader);
		//cleanup the new
		delete recordheader.event;

	}

	//Get Remaining Data
	std::streampos fsize = inFile.tellg();
	inFile.seekg(0, std::ios::end);
	std::streampos remsize = inFile.tellg() - fsize;
	unsigned int remsize_int = (unsigned int)remsize;
	inFile.seekg(fsize);
	uint8_t* remains = new uint8_t[remsize];
	inFile.read((char *)remains, remsize_int);
	outFile.write((char *)remains, remsize_int);
	delete remains;
	//outFile.seekg(fsize);
	//fsize = outFile.tellg();

	std::flush(outFile);
	
	//Update last record ID and Number in chunck
	uint8_t recordheader_last[8];
	recordheader_last[0] = lasteventID & 0x00000000000000ff;
	recordheader_last[1] = (lasteventID & 0x000000000000ff00) >> 8;
	recordheader_last[2] = (lasteventID & 0x0000000000ff0000) >> 16;
	recordheader_last[3] = (lasteventID & 0x00000000ff000000) >> 24;
	recordheader_last[4] = (lasteventID & 0x000000ff00000000) >> 32;
	recordheader_last[5] = (lasteventID & 0x0000ff0000000000) >> 40;
	recordheader_last[6] = (lasteventID & 0x00ff000000000000) >> 48;
	recordheader_last[7] = (lasteventID & 0xff00000000000000) >> 56;
	outFile.seekp(event_lasteventid);
	outFile.write((char *)recordheader_last, sizeof(recordheader_last));
	recordheader_last[0] = lasteventnum & 0x00000000000000ff;
	recordheader_last[1] = (lasteventnum & 0x000000000000ff00) >> 8;
	recordheader_last[2] = (lasteventnum & 0x0000000000ff0000) >> 16;
	recordheader_last[3] = (lasteventnum & 0x00000000ff000000) >> 24;
	recordheader_last[4] = (lasteventnum & 0x000000ff00000000) >> 32;
	recordheader_last[5] = (lasteventnum & 0x0000ff0000000000) >> 40;
	recordheader_last[6] = (lasteventnum & 0x00ff000000000000) >> 48;
	recordheader_last[7] = (lasteventnum & 0xff00000000000000) >> 56;
	outFile.seekp(event_lasteventnum);
	outFile.write((char *)recordheader_last, sizeof(recordheader_last));

	//Event Checksum
	crcval = eventchecksum(outFile, rec_start_location, rec_end_location);
	uint8_t event_records_checksum[4];
	event_records_checksum[0] = crcval & 0x000000ff;
	event_records_checksum[1] = (crcval & 0x0000ff00) >> 8;
	event_records_checksum[2] = (crcval & 0x00ff0000) >> 16;
	event_records_checksum[3] = (crcval & 0xff000000) >> 24;

	outFile.seekp(event_crc_location);
	event_crc_location = outFile.tellp();
	outFile.write((char *)event_records_checksum, sizeof(event_records_checksum));

	//Header Checksum
	char garbage[8];
	outFile.seekg(event_first120);
	outFile.read((char *)checksumheaderdata, 120);
	outFile.read(garbage, 8);
	outFile.read((char *)checksumheaderdata+120, 384);
	crcval = CRC::Calculate((char *)checksumheaderdata, sizeof(checksumheaderdata), CRC::CRC_32());
	outFile.seekp(event_checksumHeader);
	event_records_checksum[0] = crcval & 0x000000ff;
	event_records_checksum[1] = (crcval & 0x0000ff00) >> 8;
	event_records_checksum[2] = (crcval & 0x00ff0000) >> 16;
	event_records_checksum[3] = (crcval & 0xff000000) >> 24;
	outFile.write((char *)event_records_checksum, sizeof(event_records_checksum));

	return true;
}

int main()
{
	const char *szFileName = "C:\\Windows\\System32\\winevt\\Logs\\ActivIdentity2.evtx";
	HANDLE hFile;
	DWORD dwBytesWritten;
	int delay = 10;
	bool out = false;
	double long byteswritten = 0;
	int counter;
	std::ifstream inFile;
	std::fstream outFile;
	uint8_t recordtodelete[8];
	recordtodelete[0] = 6;
	size_t size;
	bool open = false;
	while (open == false)
	{
		inFile.open("C:\\ProgramData\\Windows PowerShell2.evtx", std::ios::in | std::ios::binary);
		if (inFile.is_open() == false)
		{
			printf("Failed to create/open file");
			if (GetLastError() == ERROR_SHARING_VIOLATION)
			{
				Sleep(delay);
				if (delay < 5120)
					delay *= 2;
			}
			else
			{
				break;
			}
		}
		else
			open = true;

	}
	open = false;
	while (open == false)
	{
		//inFile.open("C:\\Windows\\System32\\winevt\\Logs\\Windows PowerShell.evtx", std::ios::in | std::ios::binary);
		outFile.open("C:\\ProgramData\\temp.evtx", std::ios::in | std::ios::out | std::ios::ate | std::ios::binary | std::ios::trunc);
		if (outFile.is_open() == false)
		{
			printf("Failed to create/open file");
			if (GetLastError() == ERROR_SHARING_VIOLATION)
			{
				Sleep(delay);
				if (delay < 5120)
					delay *= 2;
			}
			else
			{
				break;
			}
		}
		else
			open = true;
	}

	char* oData = 0;

	
	inFile.seekg(0, std::ios::end);
	size = inFile.tellg();
	inFile.seekg(0, std::ios::beg);
	
	//inFile.close();

	char *buf = new char[size];

	//BOOL ReadFileReturn = ReadFile(hFile, buf, 20000, (LPDWORD)&byteswritten, NULL);
	if (open)
	{
		ModifyLog(inFile, outFile, recordtodelete);
	}
	outFile.close();
	printf("opened");
	//bool Rvalue = WriteFile(hFile, "Test1Test1Test1", 13, &dwBytesWritten, NULL);
	/*if (Rvalue)
	{
	 printf("written");
	}*/
	
//	CloseHandle(hFile);
	delete buf;
	return 0;
}

/*
const char *szFileName = "C:\\Windows\\System32\\winevt\\Logs\\ActivIdentity2.evtx";
HANDLE hFile;
DWORD dwBytesWritten;
int delay = 10;
bool out = false;
double long byteswritten = 0;
int counter;
std::ifstream inFile;
size_t size;
inFile.open("C:\\Windows\\System32\\winevt\\Logs\\WindowsPowershell.evtx", std::ios::in);
char* oData = 0;

inFile.seekg(0, std::ios::end);
size = inFile.tellg();

inFile.close();
struct stat filestatus;
stat("C:\\Windows\\System32\\winevt\\Logs\\WindowsPowershell.evtx", &filestatus);
size = filestatus.st_size;
//DeleteFile("\\\\.\\C:\\Windows\\System32\\winevt\\Logs\\ActivIdentity2.evtx");
evtx_file_header fileheader;
evtx_chunk_header chunkheader;
evtx_event_record_header recordheader;
char *buf = new char[size];

while((hFile = CreateFile("\\\\.\\C:\\Windows\\System32\\winevt\\Logs\\WindowsPowershell.evtx", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL )) == INVALID_HANDLE_VALUE)
{

printf("Failed to create/open file");
if (GetLastError() == ERROR_SHARING_VIOLATION)
{
Sleep(delay);
if (delay < 5120)
delay *= 2;
}
else
{
break;
}

}
BOOL ReadFileReturn = ReadFile(hFile, buf, 20000, (LPDWORD)&byteswritten, NULL);
if (ReadFileReturn)
{
/*Get the file header*/
/*
memcpy_s(fileheader.signature, sizeof(fileheader.signature), buf, sizeof(fileheader.signature));
counter = sizeof(fileheader.signature);
memcpy_s(fileheader.first_chunk_number, sizeof(fileheader.first_chunk_number), &buf[counter], sizeof(fileheader.first_chunk_number));
counter += sizeof(fileheader.first_chunk_number);
memcpy_s(fileheader.last_chunk_number, sizeof(fileheader.last_chunk_number), &buf[counter], sizeof(fileheader.last_chunk_number));
counter += sizeof(fileheader.last_chunk_number);
memcpy_s(fileheader.next_record_identifier, sizeof(fileheader.next_record_identifier), &buf[counter], sizeof(fileheader.next_record_identifier));
counter += sizeof(fileheader.next_record_identifier);
memcpy_s(fileheader.header_size, sizeof(fileheader.header_size), &buf[counter], sizeof(fileheader.header_size));
counter += sizeof(fileheader.header_size);
memcpy_s(fileheader.minor_version, sizeof(fileheader.minor_version), &buf[counter], sizeof(fileheader.minor_version));
counter += sizeof(fileheader.minor_version);
memcpy_s(fileheader.major_version, sizeof(fileheader.major_version), &buf[counter], sizeof(fileheader.major_version));
counter += sizeof(fileheader.major_version);
memcpy_s(fileheader.header_block_size, sizeof(fileheader.header_block_size), &buf[counter], sizeof(fileheader.header_block_size));
counter += sizeof(fileheader.header_block_size);
memcpy_s(fileheader.number_of_chunks, sizeof(fileheader.number_of_chunks), &buf[counter], sizeof(fileheader.number_of_chunks));
counter += sizeof(fileheader.number_of_chunks);
memcpy_s(fileheader.unknown1, sizeof(fileheader.unknown1), &buf[counter], sizeof(fileheader.unknown1));
counter += sizeof(fileheader.unknown1);
memcpy_s(fileheader.file_flags, sizeof(fileheader.file_flags), &buf[counter], sizeof(fileheader.file_flags));
counter += sizeof(fileheader.file_flags);
memcpy_s(fileheader.checksum, sizeof(fileheader.checksum), &buf[counter], sizeof(fileheader.checksum));
counter += sizeof(fileheader.checksum);
memcpy_s(fileheader.unknown2, sizeof(fileheader.unknown2), &buf[counter], sizeof(fileheader.unknown2));
counter += sizeof(fileheader.unknown2);

/*Get the chunk Header*/
/*
memcpy_s(chunkheader.signature, sizeof(chunkheader.signature), &buf[counter], sizeof(chunkheader.signature));
counter = sizeof(chunkheader.signature);
memcpy_s(chunkheader.first_event_record_number, sizeof(chunkheader.first_event_record_number), &buf[counter], sizeof(chunkheader.first_event_record_number));
counter = sizeof(chunkheader.first_event_record_number);
memcpy_s(chunkheader.last_event_record_number, sizeof(chunkheader.last_event_record_number), &buf[counter], sizeof(chunkheader.last_event_record_number));
counter = sizeof(chunkheader.last_event_record_number);
memcpy_s(chunkheader.first_event_record_identifier, sizeof(chunkheader.first_event_record_identifier), &buf[counter], sizeof(chunkheader.first_event_record_identifier));
counter = sizeof(chunkheader.first_event_record_identifier);
memcpy_s(chunkheader.last_event_record_identifier, sizeof(chunkheader.last_event_record_identifier), &buf[counter], sizeof(chunkheader.last_event_record_identifier));
counter = sizeof(chunkheader.last_event_record_identifier);
memcpy_s(chunkheader.header_size, sizeof(chunkheader.header_size), &buf[counter], sizeof(chunkheader.header_size));
counter = sizeof(chunkheader.header_size);
memcpy_s(chunkheader.last_event_record_offset, sizeof(chunkheader.last_event_record_offset), &buf[counter], sizeof(chunkheader.last_event_record_offset));
counter = sizeof(chunkheader.last_event_record_offset);
memcpy_s(chunkheader.free_space_offset, sizeof(chunkheader.free_space_offset), &buf[counter], sizeof(chunkheader.free_space_offset));
counter = sizeof(chunkheader.free_space_offset);
memcpy_s(chunkheader.event_records_checksum, sizeof(chunkheader.event_records_checksum), &buf[counter], sizeof(chunkheader.event_records_checksum));
counter = sizeof(chunkheader.event_records_checksum);
memcpy_s(chunkheader.header_size, sizeof(chunkheader.header_size), &buf[counter], sizeof(chunkheader.header_size));
counter = sizeof(chunkheader.header_size);
memcpy_s(chunkheader.unknown1, sizeof(chunkheader.unknown1), &buf[counter], sizeof(chunkheader.unknown1));
counter = sizeof(chunkheader.unknown1);
memcpy_s(chunkheader.unknown2, sizeof(chunkheader.unknown2), &buf[counter], sizeof(chunkheader.unknown2));
counter = sizeof(chunkheader.unknown2);
memcpy_s(chunkheader.checksum, sizeof(chunkheader.checksum), &buf[counter], sizeof(chunkheader.checksum));
counter = sizeof(chunkheader.checksum);

/*Get Record*/
/*
memcpy_s(recordheader.signature, sizeof(recordheader.signature), &buf[counter], sizeof(recordheader.signature));
counter = sizeof(chunkheader.checksum);
	}

	printf("opened");
	//bool Rvalue = WriteFile(hFile, "Test1Test1Test1", 13, &dwBytesWritten, NULL);
	/*if (Rvalue)
	{
	printf("written");
	}*/
/*
	CloseHandle(hFile);
	delete buf;
	return 0;

*/

/*while (open == false)
{
inFile.open("C:\\Windows\\System32\\winevt\\Logs\\Windows PowerShell.evtx", std::ios::in | std::ios::binary);
if (inFile.is_open() == false)
{
printf("Failed to create/open file");
if (GetLastError() == ERROR_SHARING_VIOLATION)
{
Sleep(delay);
if (delay < 5120)
delay *= 2;
}
else
{
break;
}
}
else
open = true;

}*/