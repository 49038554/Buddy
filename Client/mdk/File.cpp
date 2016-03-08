#include "File.h"
#include <cstdio>
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include   <unistd.h>                     //chdir() 
#include   <sys/stat.h>                 //mkdir() 
#include   <sys/types.h>               //mkdir() 
#include   <dirent.h>					//closedir()
#endif


namespace mdk
{

bool File::CreateDir(const std::string &path)
{
	int startPos = 0;
	int pos = path.find( '/', startPos );
	std::string dir;
	while ( true )
	{
		if ( -1 == pos ) dir = path;
		else dir.assign( path, 0, pos );
		if ( -1 == access( dir.c_str(), 0 ) )
		{
#ifdef WIN32
			if( 0 != mkdir(dir.c_str()) ) return false;
#else
			umask(0);
			if( 0 != mkdir(strDir, 0777) ) return false;
			umask(0);
			chmod(strDir,0777);
#endif
		}
		if ( -1 == pos ) break;
		startPos = pos + 1;
		pos = path.find( '/', startPos );
	}

	return true;
}

bool File::Exist(const std::string &name)
{
	if ( 0 == access( name.c_str(), 0 ) ) 
	{
#ifndef WIN32
		umask(0);
		chmod(strFile,0777);
#endif
		return true;
	}

	return false;
}

File::File()
{
	m_fp = NULL;
	m_isEnd = false;
}

File::File(const std::string &dir, const std::string &name)
{
	m_dir = dir;
	m_name = name;
	m_fp = NULL;
	m_isEnd = false;
}

File::~File()
{
}

File::Error File::Open(const std::string &dir, const std::string &name, File::Action act, File::DataFromat fromat)
{
	if ( NULL != m_fp ) return File::opened;

	m_dir = dir;
	m_name = name;
	return Open(act, fromat);
}

File::Error File::Open(File::Action act, File::DataFromat fromat)
{
	if ( NULL != m_fp ) return File::opened;
	if ( !File::Exist(m_dir) ) 
	{
		if ( !CreateDir(m_dir) ) return File::dirNoExist;
	}
	std::string filename = m_dir + "/" + m_name;
	std::string mode;
	if ( File::read == act ) mode = "r";
	else if ( File::write == act ) mode = "w";
	else if ( File::add == act ) mode = "a";
	else if ( File::readWrite == act ) mode = "r+";

	if ( File::assii == fromat ) mode = mode + "b";

	m_fp = fopen(filename.c_str(), mode.c_str());
	if ( NULL != m_fp ) 
	{
		m_act = act;
		m_fromat = fromat;
		ToTail();
		ToHeader();
	}
	if ( NULL != m_fp ) return File::success;
	if ( !File::Exist(filename) ) return File::fileNoExist;

	return File::faild;
}

bool File::Close()
{
	if ( NULL == m_fp ) return true;
	fclose(m_fp);
	m_fp = NULL;
	m_isEnd = false;

	return true;
}

File::Error File::ToHeader()
{
	if ( NULL == m_fp ) return File::unopen;
	if ( File::read != m_act && File::readWrite != m_act ) return File::refuse;

	if ( 0 == fseek(m_fp, SEEK_SET, 0) ) 
	{
		m_pos = 1;
		return File::success;
	}

	return File::faild;
}

File::Error File::ToTail()
{
	if ( NULL == m_fp ) return File::unopen;
	if ( File::read != m_act && File::readWrite != m_act ) return File::refuse;

	int count = ftell(m_fp);
	if ( 0 == fseek(m_fp, SEEK_END, 0) ) 
	{
		m_size = ftell(m_fp) + 1;
		m_pos = m_size;
		return File::success;
	}

	return File::faild;
}

File::Error File::Move(int pos)
{
	if ( NULL == m_fp ) return File::unopen;
	if ( File::read != m_act && File::readWrite != m_act ) return File::refuse;
	if ( m_pos + pos > m_size + 1 ) return File::outPos;
	if ( m_pos + pos <= 0 ) return File::outPos;

	int ret;
	if ( pos < 0 )
	{
		int unpos = m_pos + pos - 1;
		fseek(m_fp, unpos, SEEK_SET);
	}
	else
	{
		fseek(m_fp, pos, SEEK_CUR);
	}
	m_pos += pos;
	if ( m_pos == m_size + 1 )
	{
		m_isEnd = true;
	}
	else
	{
		m_isEnd = false;
	}

	return File::success;
}

bool File::IsEnd()
{
	return m_isEnd;
}

File::Error File::Read(void *content, int count)
{
	if ( NULL == m_fp ) return File::unopen;
	if ( 0 >= count ) return File::errorParam;
	if ( File::write == m_act || File::add == m_act ) return File::refuse;

	int ret = fread(content, sizeof(char), count, m_fp);
	if ( ret < count )
	{
		if ( feof ) 
		{
			m_isEnd = true;
		}
	}
	if ( 0 >= ret ) return File::faild;
	m_pos += ret;
	if ( ret < count ) return File::unfinished;

	return File::success;
}

File::Error File::Write(void *content, int count)
{
	if ( NULL == m_fp ) return File::unopen;
	if ( 0 >= count ) return File::errorParam;
	if ( File::read == m_act ) return File::refuse;

	int ret = fwrite(content, sizeof(char), count, m_fp);
	if ( count > ret ) return File::faild;
	m_act = File::write;

	return File::success;
}

}
