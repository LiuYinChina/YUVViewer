
//	--------------------------------------------------------------
//	
//	Copyright (C) 2009 - All Rights Reserved.
//	
//	Author:		LiuYin
//	File:		Utility
//	Version: 	1.0
//	Date: 		2009-8-10
//	
//	Purpose:	
//	
//	--------------------------------------------------------------

#ifndef Utility_H
#define Utility_H

//////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
struct Pair
{
	T1 first;
	T2 second;
};

template <typename PairT, typename ParamT, typename ComparerT>
long Find(const PairT *pPair, DWORD dwCount, ParamT pParam, ComparerT &comparer)
{
	long lIndex = -1;
	for (long i=0; i<dwCount; ++i) {
		if (comparer(pPair[i], pParam)) {
			lIndex = i;
			break;
		}
	}
	return lIndex;
}


template <typename Type>
size_t ItemCount(const Type &tArray)
{
	return sizeof(tArray)/sizeof(tArray[0]);
}

template <typename Type>
size_t ByteCount(const Type &tArray)
{
	return sizeof(tArray)/sizeof(char);
}

//////////////////////////////////////////////////////////////////////////

template <class T>
class CGuard
{
public:
	CGuard(T *pLocker)
		:	m_pLocker(0)
	{
		Lock(pLocker);
	}
	CGuard(T &locker)
		:	m_pLocker(0)
	{
		Lock(locker);
	}
	~CGuard()
	{
		Unlock();
	}

	void Lock(T *pLocker)
	{
		m_pLocker = pLocker;
		if (m_pLocker) {
			m_pLocker->Lock();
		}
	}
	void Lock(T &locker)
	{
		Lock(&locker);
	}
	void Unlock()
	{
		if (m_pLocker) {
			m_pLocker->Unlock();
			m_pLocker = 0;
		}
	}

private:
	T *m_pLocker;
};

//////////////////////////////////////////////////////////////////////////

template <class TargetType, class SourceType>
TargetType ForceCast(SourceType source)
{
	union TypeUnion {
		TargetType targetType;
		SourceType sourceType;
	} _TypeUnion;

	_TypeUnion.sourceType = source;
	return _TypeUnion.targetType;
}

#ifndef MAKEUNIQUEID
	#define MAKEUNIQUEID(ch0, ch1, ch2, ch3) ((DWORD)(BYTE)(ch0)|((DWORD)(BYTE)(ch1)<<8)|((DWORD)(BYTE)(ch2)<<16)|((DWORD)(BYTE)(ch3)<<24))
#endif

//////////////////////////////////////////////////////////////////////////

#endif
