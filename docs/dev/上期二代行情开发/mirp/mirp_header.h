/*!****************************************************************************
 @note   Copyright (coffee), 2005-2014, Shengli Tech. Co., Ltd.
 @file   guava_quote.h
 @date   2014/8/27   12:44
 @author zhou.hu
 
 @brief   ���ļ���GUAVA�����鲥�ӿڵ�ʾ������

 @note 
******************************************************************************/
#pragma once

#include "stdint.h"

#pragma pack(push, 1)

/**
 * MIRP������24����
 * ��MIRP����������MIRP��������������MIRP�������������������
 * ��
 *
 */
class mirp_header_t
{
	/*
	* �������������4��4������4�(Flag&0x0F)�������
	* ������1��4������(��4�,Flag&0x10)���������
	* ������0������������������
	* �1���������������������3���������
	*/
	uint8_t Flag; 

	/*
	 * ����������������������
	 */
	int8_t TypeID;	

	/*
	 * ��������������
	 */
	uint16_t Length;  


	/**
	  * ����������������������
	  *		* ���������������������
	**/
	int32_t PacketNo; 

	/*
	 * ������������������
	 */
	int16_t TopicID;  

	/*
	 * �������������������������������������
	 *
	 */
	uint16_t SnapMillisec;  

	/*
	 * ����������������������������������
	 */
	int32_t SnapNo;  

	/*
	 * ������������������������������������
	 */
	uint32_t SnapTime;  

	/*
	 * �1980�01�01�������������������
	 */
	uint16_t CommPhaseNo;  

	/*
	 * * ������������������������
	 *	* ������������������0�������������1�
	 */
	int8_t CenterChangeNo; 

	/*
	 * �����
	 */
	int8_t Reserved;  
};

#pragma pack(pop)
