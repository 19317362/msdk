#include "StdAfx.h"
#include "Confusion.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
static R_RSA_PRIVATE_KEY PRIVATE_KEY = 
{
	512,
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0xb5,0xf4,0x53,0xb5,0xaa,0xc3,0x9b,0x76,
	 0x47,0xe7,0x55,0x79,0xee,0xbd,0x56,0x5d,0x6c,0x81,0x41,0xcf,
	 0xd4,0x65,0xfc,0xff,0x4d,0x2a,0x61,0xf3,0x7c,0x26,0xc4,0x1c,
	 0x2e,0xfc,0x5c,0xb7,0x08,0xfc,0x6f,0x88,0xa4,0x42,0xc8,0xd6,
	 0x47,0xe0,0x33,0x51,0xbf,0x07,0xca,0xcd,0xce,0x99,0x45,0x2e,
	 0x6c,0xd1,0x39,0xa9,0xcd,0x9c,0x97,0xbf},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x33,0xaf,0x4b,0x77,0xf7,0x21,0xc9,0x2f,
	 0x38,0x0d,0x86,0x42,0x34,0x86,0xa3,0xd9,0xb5,0xd0,0xa1,0x99,
	 0x1f,0x84,0x38,0xf1,0x6c,0x41,0xf5,0xc0,0xec,0x2b,0x36,0xd9,
	 0x2f,0x64,0x0d,0xc7,0x49,0x4f,0x2a,0x95,0x33,0xb6,0xef,0x3a,
	 0x81,0xe3,0xad,0x49,0x6f,0x82,0xd4,0xa2,0x8a,0x5e,0xe0,0x2d,
	 0x45,0xa3,0x35,0x4f,0xd2,0xe6,0xbe,0x81},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xee,0xa1,0x76,0xd5,
	  0x4c,0x3c,0x12,0x42,0x5a,0x85,0x3f,0xd9,0x79,0x79,0x79,0x77,
	  0x24,0xdb,0xa1,0x77,0x28,0xdb,0x24,0x63,0x8e,0xdc,0xfc,0xd2,
	  0xf3,0x1b,0x4f,0xc9},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc3,0x32,0xc8,0x3e,
	 0x0c,0xf8,0xdb,0x82,0x60,0xe5,0x6d,0xe7,0x8e,0xf2,0xe4,0xcc,
	 0x74,0x61,0x21,0x74,0x25,0x02,0x14,0xbe,0x77,0x32,0x06,0x3b,
	 0x2e,0xaf,0x3f,0x47}},
	{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xac,0x87,0x72,0xda,
	  0x5e,0x66,0xe4,0x30,0xb3,0x2f,0xb9,0xed,0x2b,0x65,0x87,0x4b,
	  0x7c,0x0b,0x8b,0x5f,0x12,0xdb,0xf6,0x20,0x41,0x0f,0x47,0x7f,
	  0xe1,0x22,0xb2,0xf9},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xce,0x69,0x04,
	 0x59,0x7b,0xbb,0x60,0x9c,0xad,0x40,0x39,0xb8,0x1e,0xa9,0x2e,
	 0xba,0x16,0xbc,0x4f,0x7c,0x8f,0xf8,0x36,0x95,0xdd,0xa3,0xcb,
	 0x08,0xaf,0x2d,0x99}},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1d,0xb2,0x27,0xbb,
	 0xf6,0xfe,0xb7,0x70,0xcf,0xe0,0xfc,0x6b,0xea,0xc8,0x7f,0x0c,
	 0x7f,0xbf,0xdb,0x51,0xce,0xa2,0x22,0xa6,0xe0,0x0f,0x93,0x29,
	 0xad,0x44,0xe8,0xfd}
};
/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillModulus(BYTE lpModulus[MAX_RSA_MODULUS_LEN])
{
	Confusion(PRIVATE_KEY.modulus, MAX_RSA_MODULUS_LEN, 5, CONFUSION_ADD);
	CopyMemory(lpModulus, PRIVATE_KEY.modulus, MAX_RSA_MODULUS_LEN);
	ZeroMemory(lpModulus, 64);
	*(DWORD*)(lpModulus + 64)  = 0x6031D9C8;
	*(DWORD*)(lpModulus + 108) = 0x0037CE2C;
	*(DWORD*)(lpModulus + 72)  = 0x3DE17656;
	*(DWORD*)(lpModulus + 76)  = 0xFBA89A89;
	*(DWORD*)(lpModulus + 84)  = 0x3247FB68;
	*(DWORD*)(lpModulus + 88)  = 0x587D5693;
	*(DWORD*)(lpModulus + 92)  = 0xEA5CAE31;
	*(DWORD*)(lpModulus + 96)  = 0xDB9C51D3;
	*(DWORD*)(lpModulus + 124) = 0xBDF9770A;
	*(DWORD*)(lpModulus + 68)  = 0xB2C54CA3;
	*(DWORD*)(lpModulus + 80)  = 0x603B08B2;
	*(DWORD*)(lpModulus + 100) = 0x73794EE7;
	*(DWORD*)(lpModulus + 104) = 0x80B39C83;
	*(DWORD*)(lpModulus + 112) = 0xF4CF9D45;
	*(DWORD*)(lpModulus + 116) = 0xC5145330;
	*(DWORD*)(lpModulus + 120) = 0x1CABAA6C;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillPublicExponent(BYTE lpPublicExponent[MAX_RSA_MODULUS_LEN])
{
	Confusion(PRIVATE_KEY.publicExponent, MAX_RSA_MODULUS_LEN, 3, CONFUSION_SUB);
	CopyMemory(lpPublicExponent, PRIVATE_KEY.publicExponent, MAX_RSA_MODULUS_LEN);
	ZeroMemory(lpPublicExponent, 125);
	lpPublicExponent[127] = 7;
	Confusion(lpPublicExponent + 127, 1, 6, CONFUSION_SUB);
	lpPublicExponent[125] = 11;
	Confusion(lpPublicExponent + 125, 2, 8, CONFUSION_SHR);
	lpPublicExponent[125] += 1;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillExponent(BYTE lpExponent[MAX_RSA_MODULUS_LEN])
{
	Confusion(PRIVATE_KEY.exponent, MAX_RSA_MODULUS_LEN, 2, CONFUSION_MUL);
	CopyMemory(lpExponent, PRIVATE_KEY.exponent, MAX_RSA_MODULUS_LEN);
	ZeroMemory(lpExponent, 64);
	
	*(DWORD*)(lpExponent + 68)  = 0xED633EA6;
	*(DWORD*)(lpExponent + 72)  = 0xC9612FE6;
	*(DWORD*)(lpExponent + 100) = 0x3C96C3B7;
	*(DWORD*)(lpExponent + 80)  = 0x8E9578E3;
	*(DWORD*)(lpExponent + 84)  = 0x99B93CCF;
	*(DWORD*)(lpExponent + 108) = 0xACA3E9B8;
	*(DWORD*)(lpExponent + 88)  = 0xC71F8707;
	*(DWORD*)(lpExponent + 76)  = 0x6697B477;
	*(DWORD*)(lpExponent + 92)  = 0xA1E7E4F9;
	*(DWORD*)(lpExponent + 96)  = 0xB8A02B43;
	*(DWORD*)(lpExponent + 124) = 0x654AB93F;
	*(DWORD*)(lpExponent + 104) = 0x5B9E6DC7;
	*(DWORD*)(lpExponent + 112) = 0x63E854FF;
	*(DWORD*)(lpExponent + 116) = 0x1A75A5F5;
	*(DWORD*)(lpExponent + 120) = 0x10038895;
	*(DWORD*)(lpExponent + 64)  = 0xDDD3115E;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillPrime1(BYTE lpPrime1[MAX_RSA_PRIME_LEN])
{
	Confusion(PRIVATE_KEY.prime[0], MAX_RSA_PRIME_LEN, 7, CONFUSION_DIV);
	CopyMemory(lpPrime1, PRIVATE_KEY.prime[0], MAX_RSA_PRIME_LEN);
	ZeroMemory(lpPrime1, 32);
	*(DWORD*)(lpPrime1 + 36) = 0x76EE4F05;
	*(DWORD*)(lpPrime1 + 60) = 0x6359E2F4;
	*(DWORD*)(lpPrime1 + 40) = 0x9F165CDA;
	*(DWORD*)(lpPrime1 + 32) = 0x07C6CBF8;
	*(DWORD*)(lpPrime1 + 48) = 0x1E63C7F2;
	*(DWORD*)(lpPrime1 + 52) = 0xFDA7024A;
	*(DWORD*)(lpPrime1 + 56) = 0xBD054210;
	*(DWORD*)(lpPrime1 + 44) = 0xEDB9EE87;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillPrime2(BYTE lpPrime2[MAX_RSA_PRIME_LEN])
{
	Confusion(PRIVATE_KEY.prime[1], MAX_RSA_PRIME_LEN, 3, CONFUSION_SHL);
	CopyMemory(lpPrime2, PRIVATE_KEY.prime[1], MAX_RSA_PRIME_LEN);
	ZeroMemory(lpPrime2, 32);
	*(DWORD*)(lpPrime2 + 32) = 0xBA00AACE;
	*(DWORD*)(lpPrime2 + 60) = 0x5F5A8480;
	*(DWORD*)(lpPrime2 + 40) = 0xF678CF50;
	*(DWORD*)(lpPrime2 + 44) = 0x4FD3124D;
	*(DWORD*)(lpPrime2 + 56) = 0x1589A9E1;
	*(DWORD*)(lpPrime2 + 48) = 0x593359BB;
	*(DWORD*)(lpPrime2 + 52) = 0xB40FB823;
	*(DWORD*)(lpPrime2 + 36) = 0xEA40A5BD;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillPrimeExponent1(BYTE lpPrimeExponent1[MAX_RSA_PRIME_LEN])
{
	Confusion(PRIVATE_KEY.primeExponent[0], MAX_RSA_PRIME_LEN, 2, CONFUSION_SHR);
	CopyMemory(lpPrimeExponent1, PRIVATE_KEY.primeExponent[0], MAX_RSA_PRIME_LEN);
	ZeroMemory(lpPrimeExponent1, 32);
	*(DWORD*)(lpPrimeExponent1 + 32) = 0x538CB652;
	*(DWORD*)(lpPrimeExponent1 + 44) = 0x0F48C9D9;
	*(DWORD*)(lpPrimeExponent1 + 40) = 0xCB97C9DD;
	*(DWORD*)(lpPrimeExponent1 + 56) = 0x5F13C19F;
	*(DWORD*)(lpPrimeExponent1 + 52) = 0x52C6BE33;
	*(DWORD*)(lpPrimeExponent1 + 36) = 0xB6AD3471;
	*(DWORD*)(lpPrimeExponent1 + 60) = 0xB950B69F;
	*(DWORD*)(lpPrimeExponent1 + 48) = 0x24B2930F;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillPrimeExponent2(BYTE lpPrimeExponent2[MAX_RSA_PRIME_LEN])
{
	Confusion(PRIVATE_KEY.primeExponent[1], MAX_RSA_PRIME_LEN, 7, CONFUSION_SHL);
	CopyMemory(lpPrimeExponent2, PRIVATE_KEY.primeExponent[1], MAX_RSA_PRIME_LEN);
	ZeroMemory(lpPrimeExponent2, 32);
	*(DWORD*)(lpPrimeExponent2 + 32) = 0x82EDD933;
	*(DWORD*)(lpPrimeExponent2 + 36) = 0x6018F0EC;
	*(DWORD*)(lpPrimeExponent2 + 56) = 0xA896AFCD;
	*(DWORD*)(lpPrimeExponent2 + 44) = 0x5C10CE67;
	*(DWORD*)(lpPrimeExponent2 + 48) = 0x8BE8D991;
	*(DWORD*)(lpPrimeExponent2 + 40) = 0x3A0CD471;
	*(DWORD*)(lpPrimeExponent2 + 60) = 0xAB536D61;
	*(DWORD*)(lpPrimeExponent2 + 52) = 0x6A09EE35;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::FillCoefficient(BYTE lpCoefficient[MAX_RSA_PRIME_LEN])
{
	Confusion(PRIVATE_KEY.coefficient, MAX_RSA_PRIME_LEN, 3, CONFUSION_SHL);
	CopyMemory(lpCoefficient, PRIVATE_KEY.coefficient, MAX_RSA_PRIME_LEN);
	ZeroMemory(lpCoefficient, 32);
	*(DWORD*)(lpCoefficient + 40) = 0x4315FF2C;
	*(DWORD*)(lpCoefficient + 36) = 0xFA1FE9EF;
	*(DWORD*)(lpCoefficient + 44) = 0xCBDAADB5;
	*(DWORD*)(lpCoefficient + 32) = 0xFE8A5ECF;
	*(DWORD*)(lpCoefficient + 56) = 0xBA255B2C;
	*(DWORD*)(lpCoefficient + 52) = 0x7A86BE5A;
	*(DWORD*)(lpCoefficient + 48) = 0x259D5137;
	*(DWORD*)(lpCoefficient + 60) = 0x4E8175FF;
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::ReverseBuffer(LPBYTE lpBuffer, DWORD dwLen)
{
	DWORD dwHalf = dwLen / 2;
	BYTE byTemp;
	for (DWORD i = 0; i < dwHalf; i++)
	{
		byTemp = lpBuffer[i];
		lpBuffer[i] = lpBuffer[dwLen - i - 1];
		lpBuffer[dwLen - i - 1] = byTemp;
	}
}

/////////////////////////////////////////////////////////////////////////////
VOID CConfusion::Confusion(LPBYTE lpBuffer, DWORD dwLen, DWORD dwFactor, DWORD dwOperation)
{
	DWORD i;
	switch (dwOperation)
	{
	case CONFUSION_ADD:
		for (i = 0; i < dwLen; i++)
		{
			lpBuffer[i] += (BYTE)dwFactor;
		}
		break;
	case CONFUSION_SUB:
		for (i = 0; i < dwLen; i++)
		{
			lpBuffer[i] -= (BYTE)dwFactor;
		}
		break;
	case CONFUSION_MUL:
		for (i = 0; i < dwLen; i++)
		{
			lpBuffer[i] *= (BYTE)dwFactor;
		}
		break;
	case CONFUSION_DIV:
		for (i = 0; i < dwLen; i++)
		{
			lpBuffer[i] /= (BYTE)dwFactor;
		}
		break;
	case CONFUSION_SHL:
		for (i = 0; i < dwLen; i++)
		{
			lpBuffer[i] <<= (BYTE)dwFactor;
		}
		break;
	case CONFUSION_SHR:
		for (i = 0; i < dwLen; i++)
		{
			lpBuffer[i] >>= (BYTE)dwFactor;
		}
		break;
	default:
		_ASSERT(FALSE);
	}
}