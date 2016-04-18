#pragma once
#include "ggdbextend_global.h"
#include "GCMPModel/IGCMPObject.h"
#include <QString>

//���ݿ�ȽϽ����
class GGDBEXTEND_EXPORT GCMPDataBaseResult : public IGCMPResult
{
public:
    GCMPDataBaseResult() : IGCMPResult(grtDataBase){setIsLeaf(false);}
    virtual ~GCMPDataBaseResult(){}
};


//���ȽϽ����
class GGDBEXTEND_EXPORT GCMPTableResult : public IGCMPResult
{
public:
    GCMPTableResult() : IGCMPResult(grtTable){setIsLeaf(false);}
    virtual ~GCMPTableResult(){}
};

//��¼�ȽϽ����
class GGDBEXTEND_EXPORT GCMPRecordResult: public IGCMPResult
{
public:
    GCMPRecordResult() : IGCMPResult(grtRecord) {setIsLeaf(false);}
    virtual ~GCMPRecordResult() {}
};


//field�ȽϽ����
class GGDBEXTEND_EXPORT GCMPFieldResult : public IGCMPLeafResult
{
public:
    GCMPFieldResult(): IGCMPLeafResult(grtField){}
    virtual ~GCMPFieldResult() {}

public:

};


//��Ԫ��Ƚ���
class GGDBEXTEND_EXPORT GCMPFieldValueResult : public IGCMPLeafResult
{
public:
    GCMPFieldValueResult() {}
    virtual ~GCMPFieldValueResult() {}

public:
    QString fieldName() const {return m_strFieldName;}

private:
    QString m_strFieldName;
};


