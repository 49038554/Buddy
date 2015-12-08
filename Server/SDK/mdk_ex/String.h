#ifndef STRING_H
#define STRING_H

#include "ShareObject.h"
#include "FastMemoryPool.h"

namespace mdk
{


class String
{
	class Data : public ShareObject
	{
	public:
		char *m_str;
		virtual void Delete();
	};
public:
	String(void);
	String(const String &right);
	~String(void);

	static FastMemoryPool ms_pool;//�����NULL,������ڴ�ط���
	String& operator = (const String &right);
	operator char* ();
	operator unsigned char* ();
	char* c_str();
private:

	Data *m_data;
};

}


#endif //STRING_H

