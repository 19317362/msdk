#pragma once

#include <assert.h>

#include <regex> 
using namespace std::tr1;

#ifdef _UNICODE
typedef wcmatch	t_match;
typedef wregex	t_regex;
typedef wsmatch	t_wsmatch;
#else
typedef cmatch	t_match;
typedef regex	t_regex;
typedef smatch	t_wsmatch;
#endif

namespace match_strings
{



//ֱ��ͨ��������ʽ����ƥ�䣬����������ȡͳ�ƽ��
//szText		��Ҫƥ����ı�
//szRegExpr		ECMAScript������ʽ���ʽ
//pdwTotal		��������ַ
//dwSize		�����С
//bNoCase		��Сд������
DWORD GetMatchCount( LPCTSTR szText, LPCTSTR szRegExpr, LPDWORD pdwTotal, DWORD dwSize, BOOL bNoCase = FALSE);

//���������ʽ������ƥ�䣬����ͳ�ƶ�����ʽ��ƥ�����
class CRegexMatchStrings;


// �����Ǿ���ʵ��

DWORD GetMatchCount( LPCTSTR szText, LPCTSTR szRegExpr, LPDWORD pdwTotal, DWORD dwSize, BOOL bNoCase)
{		
	assert(szText);
	assert(szRegExpr);
	assert(pdwTotal);

	t_match mr; 
	t_regex rx(szRegExpr, 
		bNoCase?(regex_constants::ECMAScript|regex_constants::icase):(regex_constants::ECMAScript)); 
	regex_constants::match_flag_type ft = regex_constants::match_default; 

	DWORD dwCount=0;
	LPCTSTR it = szText;
	LPCTSTR end = szText+lstrlen(szText);
	while(regex_search(it, end, mr, rx, ft))
	{
// #ifdef _DEBUG
// 		assert(dwSize<=mr.size());
// 		TRACE("size = %d\n", mr.size());
// 		USES_CONVERSION;
// 		TRACE("[%d] find = %s, \t next = %s\n", dwCount, T2A(mr[0].first), T2A(mr[0].second));
// #endif
		for(DWORD index = 0; index<dwSize && (index+1)<mr.size(); index++)
		{
			if(mr[index+1].matched)//�±��1��ʼ��
				++(pdwTotal[index]);
		}			

		dwCount++;
		it = mr[0].second;			
	}		

	return dwCount;
}


//���������ʽ������ƥ��
class CRegexMatchStrings
{
public:
	CRegexMatchStrings()
	{
		Clear();
	}

	//����
	void Clear()
	{
		m_strRules = _T("");
		m_arTotals.RemoveAll();
	}

	//����һ��������ʽ������һ������������ͨ�����������ͳ�ƽ�����м���
	DWORD AddRegExpr(LPCTSTR szRegExpr)
	{
		//"(a)|(b)|(c)"
		DWORD dwCount = m_arTotals.GetSize();
		if(dwCount==0)
			m_strRules = szRegExpr;
		else if(dwCount==1)
		{
			CString str1 = m_strRules;
			m_strRules.Format(_T("(%s)|(%s)"), str1, szRegExpr);
		}
		else
		{
			m_strRules += _T("|(");
			m_strRules += szRegExpr;
			m_strRules += _T(")");
		}
		m_arTotals.Add(0);
		return m_arTotals.GetSize();
	}

	//ƥ����ɣ����һ�ȡƥ��Ľ���ܹ�������
	DWORD GetMatchResult(LPCTSTR szText)
	{
		GetMatchResultEx(szText, m_arTotals.GetData(), m_arTotals.GetSize());
		return  m_arTotals.GetSize();
	}

	//����ֱ�ӻ�ȡ������Ӧ��ͳ��Ƶ����
	const DWORD& operator[] (int nIndex) const
	{
		return m_arTotals[nIndex];
	}

	//һ�����ٻ�ȡƥ�䣬��������ý���ķ���
	//ע��:
	//		1����������ϣ����ȡ�Ķ�����������
	//		2���� GetMatchResult ����ֵ�ú��岻ͬ������Ƿ��ص�����ƥ���Ƶ���ܺ�
	DWORD GetMatchResultEx(LPCTSTR szText, LPDWORD pdwTotal, DWORD dwSize)
	{
		assert(dwSize<= (DWORD)(m_arTotals.GetSize()));
		return GetMatchCount(szText, m_strRules, pdwTotal, dwSize);
	}
public:
	CString m_strRules;
	CSimpleArray<DWORD> m_arTotals;
};


/* demo

//case 1:

DWORD dwTotals[3] = {0};
dwTotals[2] = -1;
match_strings::GetMatchCount(_T("abcdaabcdaaab"), _T("(ab)|(cd)"), dwTotals, sizeof(dwTotals)/sizeof(dwTotals[0]));
//ms.GetMatchCount(_T("abcdaabcdaaab"), _T("(a)|(b)|(c)"), dwTotals, sizeof(dwTotals)/sizeof(dwTotals[0]));

//case 2:

match_strings::CRegexMatchStrings ms;
ms.AddRegExpr(_T("ab"));
ms.AddRegExpr(_T("cd"));
int iRet = ms.GetMatchResult(_T("abcdaabcdaaab"));
TRACE("being txt = %s\n", _T("abcdaabcdaaab"));
for(int i=0; i<iRet; i++)
{
	TRACE("[%d] find = %d\n", i, ms[i]);
}
*/


}// namespace match_strings