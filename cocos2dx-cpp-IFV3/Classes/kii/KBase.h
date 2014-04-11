/*
 * KObject.h
 *
 *  Created on: 2014/02/25
 *      Author: guest
 */

#ifndef KBaseT_H_
#define KBaseT_H_

/**
*　@brief コールバックを保持する変数の型を合わせるため、共通の継承元として使用
*
*/
class KBase {
public:
	KBase();
	virtual ~KBase();
};

#endif /* KOBJECT_H_ */
