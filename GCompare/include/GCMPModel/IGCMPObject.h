#pragma once
#include "GCMPModel/gcmpmodel_global.h"
#include <vector>
#include <QString>

enum GCMPMODEL_EXPORT GCMPObjectType
{
    gotUnknown = -1,
    gotDBContainer = 0,
    gotDataBase,
    gotTable,
    gotRecord,
    gotField,
    gotTableStruct,
};


enum GCMPMODEL_EXPORT GCMPResultType
{
    grtUnKnown = -1,
    grtDBContainer = 0,
    grtDataBase,
    grtTable,
    grtRecord,
    grtField,
};

enum GCMPMODEL_EXPORT GCMPState
{
    gsUnknown = -1,         //未知
    gsStrictlyEqule = 0,    //严格相等，1-1的关系
    gsPartlyEqule,          //部分相等，A或B互相有缺失
    gsCompletelyDifferent,  //完全不一样
};

//比较结果
class GCMPMODEL_EXPORT IGCMPResult
{
public:
    IGCMPResult(GCMPResultType oType = grtUnKnown){m_type = oType; m_bLeaf = false; m_bLeftEmpty = false; m_bRightEmpty = false;};
    virtual ~IGCMPResult();

public:
    //外部不能释放
    std::vector<IGCMPResult*> subResults() {return m_vecSubReulst;}
    GCMPResultType type() const {return m_type;}
    bool leftEmpty() const {return m_bLeftEmpty;}
    void setLeftEmpty(bool bValue) {m_bLeftEmpty = bValue;}
    bool rightEmpty() const {return m_bRightEmpty;}
    void setRightEmpty(bool bValue) {m_bRightEmpty = bValue;}
    bool isLeaf() const {return m_bLeaf;}
    void setState(GCMPState oState) {m_oState = oState;}
    GCMPState state() const {return m_oState;}
    void addSub(IGCMPResult* pSub) {m_vecSubReulst.push_back(pSub);}
    void setIsLeaf(bool bValue) {m_bLeaf = bValue;}

private:
    std::vector<IGCMPResult*> m_vecSubReulst;  //子结果
    GCMPResultType m_type;
    bool m_bLeftEmpty;
    bool m_bRightEmpty;
    bool m_bLeaf;
    GCMPState m_oState;
};

class GCMPMODEL_EXPORT IGCMPLeafResult : public IGCMPResult
{
public:
    IGCMPLeafResult() : IGCMPResult(){setIsLeaf(true);}
    IGCMPLeafResult(GCMPResultType oType) : IGCMPResult(oType) {setIsLeaf(true);}

    QString leftValue() const {return m_strLeftValue;}
    void setLeftValue(QString strLeft) {m_strLeftValue = strLeft;}

    QString rightValue() const {return m_strRightValue;}
    void setRightValue(QString strRight) {m_strRightValue = strRight;}

protected:
    QString m_strLeftValue;
    QString m_strRightValue;
};

//比较对象
class GCMPMODEL_EXPORT IGCMPObject
{
public:
    IGCMPObject(void);
    ~IGCMPObject(void);
public:
    GCMPObjectType type() const {return m_eType;}
    void setType(const GCMPObjectType oType) {m_eType = oType;}

    //返回的IGCMPResult需要外部释放
    virtual IGCMPResult* compare(IGCMPObject* pOther) = 0;

protected:
    //判断两个Object是否可以进行比较
    virtual bool canCompare(IGCMPObject* pOther) = 0;

private:
    GCMPObjectType m_eType;
};
