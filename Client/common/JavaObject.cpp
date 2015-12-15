#include "JavaObject.h"
typedef char				int8;
typedef unsigned char		uint8;
typedef short				int16;
typedef unsigned short		uint16;
typedef int					int32;
typedef unsigned int		uint32;
#ifdef WIN32
typedef __int64				int64;
typedef unsigned __int64	uint64;
#else
#include <sys/types.h>
typedef int64_t				int64;
typedef u_int64_t			uint64;
#endif

#include <cstdio>

jobject JavaObject::NewJObject( JNIEnv *env, const char *classname, const char *signature, ... )
{
	jclass cls = env->FindClass(classname); 
	jmethodID id = env->GetMethodID(cls, "<init>", signature);
	va_list ap;
	va_start( ap, signature );
	jobject paramOut = env->NewObjectV(cls, id, ap); 
	va_end( ap );
	return paramOut;
}

JavaObject::JavaObject(JNIEnv *env, const char *classname, const char *signature, ... )
{
	m_bIsRef = false;
	m_env = env;
	m_cls = m_env->FindClass(classname); 
	jmethodID id = m_env->GetMethodID(m_cls, "<init>", signature);
	va_list ap;
	va_start( ap, signature );
	m_thisPointer = m_env->NewObjectV(m_cls, id, ap); 
	va_end( ap );
	m_env->GetJavaVM(&(m_jvm));
}

JavaObject::JavaObject(JNIEnv* env, jobject pObj, bool bCreateRef)
{
	m_bIsRef = bCreateRef;
	m_env = env;
	m_env->GetJavaVM(&(m_jvm));
	if ( m_bIsRef ) m_thisPointer = m_env->NewGlobalRef(pObj);
	else m_thisPointer = pObj;
}

JavaObject::~JavaObject()
{
	if ( m_bIsRef )
	{
		m_env->DeleteGlobalRef(m_thisPointer);
	}
}

jobject JavaObject::GetObject()
{
	return m_thisPointer;
}

char* JavaObject::GetStringMember( const char *name )
{
	jfieldID bstageID = m_env->GetFieldID(m_cls, name, "Ljava/lang/String;");
	if ( 0 == bstageID ) return NULL;
	jstring str = (jstring)m_env->GetObjectField(m_thisPointer, bstageID);
	return (char*)(m_env->GetStringUTFChars(str, 0));
}

bool JavaObject::SetStringMember( const char *name, const char *value)
{
	jfieldID bstageID = m_env->GetFieldID(m_cls, name, "Ljava/lang/String;");
	if ( 0 == bstageID ) return false;
	if ( 0 == value ) m_env->SetObjectField(m_thisPointer, bstageID, 0);
	else m_env->SetObjectField(m_thisPointer, bstageID, m_env->NewStringUTF(value));
	return true;
}

bool JavaObject::GetIntMember( const char *name, int &value )
{
	jfieldID bstageID = m_env->GetFieldID(m_cls, name, "I");
	if ( 0 == bstageID ) return false;
	value = (jint)m_env->GetIntField(m_thisPointer, bstageID);
	
	return true;
}

bool JavaObject::SetIntMember( const char *name, int value )
{
	jfieldID bstageID = m_env->GetFieldID(m_cls, name, "I");
	if ( 0 == bstageID ) return false;

	m_env->SetIntField(m_thisPointer, bstageID, value);
	
	return true;
}

bool JavaObject::SetPointerMember( const char *name, void *value )
{
	int bit = sizeof(char*);
	char point[256];
	if ( 4 == bit ) sprintf( point, "%d", (int)value );
	else sprintf( point, "%llu", (uint64)value );
	return SetStringMember(name, point);
}

bool JavaObject::GetPointerMember( const char *name, void *&value )
{
	char *point = GetStringMember(name);
	if ( NULL == point ) return false;
	int bit = sizeof(char*);
	if ( 4 == bit ) 
	{
		int p;
		sscanf( point, "%u", &p );
		value = (void*)p;
	}
	else 
	{
		uint64 p;
		sscanf( point, "%llu", &p );
		value = (void*)p;
	}

	return true;
}

void JavaObject::CallMethod( const char *methodname, const char *signature, ... )
{
	if ( 0 == m_cls ) return;
	jmethodID mid = m_env->GetMethodID(m_cls, methodname, signature);
	va_list ap;
	va_start( ap, signature );
	m_env->CallVoidMethodV(m_thisPointer, mid, ap);
	va_end( ap );
}

bool JavaObject::AttachCurrentThread()
{
	if( m_jvm->AttachCurrentThread(&m_env, 0) != JNI_OK ) return false;
	m_cls = m_env->GetObjectClass(m_thisPointer);
	return true;
}

void JavaObject::DetachCurrentThread()
{
	m_jvm->DetachCurrentThread();
}

void JavaObject::DestroyJavaVM()
{
	m_jvm->DestroyJavaVM();
}

#include <cstring>
//////////////////////////////////////////////////////////////////////////
//验证ed
namespace jni
{
/*
	jstring转char*
	char*的编码是code
	code:UTF-8,GB2312
*/
char* Jstring2Chars(JNIEnv* env, jstring jstr, const char *code)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF(code);
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);

		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

/*
	创建一个jstring对象
	内容是str
	编码是code
	code:UTF-8,GB2312
*/
jstring JString( JNIEnv* env, const char *str, const char *code )
{
	jclass jstrClass = env->FindClass("java/lang/String"); 
	jmethodID jstrConstructorID = env->GetMethodID(jstrClass, "<init>", "([BLjava/lang/String;)V");
	int len = strlen(str);
	jbyteArray bytes = env->NewByteArray(len);//建立byte数组
	env->SetByteArrayRegion(bytes, 0, len, (jbyte*)str);//将char* 转换为byte数组    
	jstring jstrEncode = env->NewStringUTF("GB2312");
	jstring jstr = (jstring)env->NewObject(jstrClass, jstrConstructorID, bytes, jstrEncode);//将byte数组转换为java String,并输出
	env->DeleteLocalRef(bytes);
	env->DeleteLocalRef(jstrEncode);
	return jstr;
}

void CallStaticIntMethod(JNIEnv* env, jclass cls, jmethodID mid, ...)
{
	va_list ap;
	va_start( ap, mid );
	env->CallStaticIntMethodV(cls, mid, ap);
	va_end( ap );
}

void Logcat( JNIEnv* env, const char *filter, const char *format, ... )
{
#ifdef _LOGCAT
	char msg[2560];
	{
		va_list ap;
		va_start( ap, format );
		vsprintf( msg, format, ap );
		va_end( ap );
	}
	jclass cls = env->FindClass("android/util/Log");
	if ( 0 == cls ) return;
	jmethodID mid = env->GetStaticMethodID(cls, "d", "(Ljava/lang/String;Ljava/lang/String;)I");

	jstring jfilter = JString(env, filter, "GB2312");
	jstring jMsg = JString(env, msg, "GB2312");
	CallStaticIntMethod(env, cls, mid, jfilter, jMsg);
	env->DeleteLocalRef(jfilter);
	env->DeleteLocalRef(jMsg);
#endif
}

//c++指针转jstring
jstring Pointer2JString(JNIEnv* env, void *pObj)
{
	int bit = sizeof(char*);
	char point[256];
	if ( 4 == bit ) sprintf( point, "%d", (int)pObj );
	else sprintf( point, "%llu", (uint64)pObj );
	jstring value = env->NewStringUTF(point);
	return value;
}

//jstring转c++指针
void* JString2Pointer(JNIEnv* env, jstring str)
{
	char *point = jni::Jstring2Chars(env, str, "GB2312");
	void *pointer = NULL;
	int bit = sizeof(char*);
	if ( 4 == bit ) 
	{
		int p;
		sscanf( point, "%u", &p );
		pointer = (void*)p;
	}
	else 
	{
		uint64 p;
		sscanf( point, "%llu", &p );
		pointer = (void*)p;
	}

	return pointer;
}
}
