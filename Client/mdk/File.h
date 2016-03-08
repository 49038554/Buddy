#ifndef MDK_FILE_H
#define MDK_FILE_H

#include <string>

namespace mdk
{

class File
{
public:
	File();
	File(const std::string &dir, const std::string &name);
	virtual ~File();

	static bool Exist(const std::string &name);
	static bool CreateDir(const std::string &path);
	enum Error
	{
		success = 0,
		faild = 1,
		opened = 2, 
		unopen = 3,
		refuse = 4,//拒绝操作，一般是打开模式决定了操作是否允许
		outPos = 5,
		errorParam = 6,
		dirNoExist = 7,
		fileNoExist = 8,
		unfinished = 9,
	};
	enum Action
	{
		read = 0,//打开文件（如果文件不存在则打开失败），读取
		write = 1,//打开文件（如果文件不存在则创建文件），从头部写入
		add = 2,//打开文件（如果文件不存在则打开失败），往文件末尾添加内容
		readWrite = 3,//打开文件后，在执行写入前可读取，一旦执行了写操作后，则不能再读取
	};
	enum DataFromat
	{
		txt = 0,//文本方式IO
		assii = 1,//二进制方式IO
	};
	Error Open(const std::string &dir, const std::string &name, Action act, DataFromat fromat);
	Error Open(Action act, DataFromat fromat);
	bool Close();
	Error ToHeader();
	Error ToTail();
	Error Move(int pos);
	bool IsEnd();
	Error Read(void *content, int count);
	Error Write(void *content, int count);

private:
	std::string m_dir;
	std::string m_name;
	FILE *m_fp;
	Action m_act;
	DataFromat m_fromat;
	unsigned int m_pos;
	unsigned int m_size;
	bool m_isEnd;
};

}

#endif //MDK_FILE_H