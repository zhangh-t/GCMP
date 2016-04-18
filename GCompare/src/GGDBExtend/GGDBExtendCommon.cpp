#include "GGDBExtend/GGDBExtendCommon.h"
#include "GLD/GLDFileUtils.h"
#include <list>

GGDBEXTEND_EXPORT GCMPFieldResult* getDBFieldState(GField* pField1, GField* pField2)
{
    if (pField1 == nullptr && pField2 == nullptr)
    {
        return nullptr;
    }

    GCMPState oState = gsUnknown;
    GCMPFieldResult* pResult = new GCMPFieldResult;
    if (pField1 != nullptr && pField2 != nullptr)
    {
        GFieldSchema* pSchema1 = pField1->schema();
        GFieldSchema* pSchema2 = pField2->schema();
        if (pSchema1 != nullptr && pSchema2 != nullptr)
        {
            if ((*pSchema1) == (*pSchema2))
            {
                oState = gsStrictlyEqule;
                pResult->setLeftValue(pField1->fieldName());
                pResult->setRightValue(pField2->fieldName());
            }
        }
    }
    else if ((pField1 == nullptr && pField2 != nullptr) || (pField1 != nullptr && pField2 == nullptr))
    {
        oState = gsCompletelyDifferent;
        bool bLeftEmpty = pField1 == nullptr;
        if (bLeftEmpty)
        {
            pResult->setLeftEmpty(true);
            pResult->setRightValue(pField2->fieldName());
        }
        else
        {
            pResult->setLeftValue(pField1->fieldName());
            pResult->setRightEmpty(true);
        }
    }

    else if (pField1 == nullptr && pField2 == nullptr)
    {
        pResult->setLeftEmpty(true);
        pResult->setRightEmpty(true);
    }

    pResult->setState(oState);
    return pResult;
}

GGDBEXTEND_EXPORT GCMPRecordResult* getRecordState(GRecord* pRecord1, GRecord* pRecord2)
{
    auto insertAndFixResultList = [](std::list<GFieldCMPResultPair*>& oList, int nPosBefor, GFieldCMPResultPair* pResult) -> void
    {
        auto iter = oList.begin();
        for (int i = 0; i < nPosBefor; ++i)
        {
            ++iter;
        }

        oList.insert(iter, pResult);
        //调整后面的序号
        for (; iter != oList.end(); ++iter)
        {
            ++((*iter)->nIndex);
        }
    };

    if (pRecord1 == nullptr && pRecord2 == nullptr)
    {
        return nullptr;
    }
    GCMPState oState = gsUnknown;
    GCMPRecordResult* pResult = new GCMPRecordResult;

    //只要有一个不为空，理论上就可以比
    if ((pRecord1 != nullptr && pRecord2 == nullptr) || (pRecord1 == nullptr && pRecord2 != nullptr))
    {
        bool bFlag = pRecord1 == nullptr;
        int nFieldCount = bFlag ? pRecord2->fieldCount() : pRecord1->fieldCount();
        for (int i = 0 ; i < nFieldCount ; ++i)
        {
            GCMPFieldValueResult* pFieldValueResult = new GCMPFieldValueResult();
            pFieldValueResult->setState(gsCompletelyDifferent);
            if (bFlag)
            {
                pFieldValueResult->setLeftEmpty(true);
                pFieldValueResult->setRightValue(pRecord2->asString(i));
            }
            else
            {
                pFieldValueResult->setRightEmpty(true);
                pFieldValueResult->setLeftValue(pRecord1->asString(i));
            }
            pResult->addSub(pFieldValueResult);
        }
    }
    else
    {
        //都不为空
        //首先需要合并字段
        //以1为基准
        std::list<GFieldCMPResultPair*> listFieldValueResults;
        int nFieldCount = pRecord1->getTable()->fieldsCount();
        for (int i = 0 ; i < nFieldCount; ++i)
        {
            GCMPFieldValueResult* pFieldValueResult = new GCMPFieldValueResult();
            GFieldCMPResultPair* pPair = new GFieldCMPResultPair;
            pFieldValueResult->setState(gsUnknown);
            pFieldValueResult->setLeftValue(pRecord1->asString(i));
            pFieldValueResult->setRightEmpty(true);
            pPair->pField = pRecord1->getTable()->getField(i);
            pPair->pFeildResult = pFieldValueResult;
            pPair->nIndex = i;
            listFieldValueResults.push_back(pPair);
        }

        oState = gsStrictlyEqule;
        GTable* pSecondTable = pRecord2->getTable();
        nFieldCount = pSecondTable->fieldsCount();
        auto iter = listFieldValueResults.begin();
        int nDiffCount = 0;
        int nSameCount = 0;
        int nLastSameFieldIndex;
        for (int i = 0 ; i < nFieldCount; ++i)
        {
            GCMPFieldValueResult* pNewFieldValueResult = nullptr;
            GField* pSeconField = pSecondTable->getField(i);
            iter = std::find_if(listFieldValueResults.begin(), listFieldValueResults.end(), GFieldFinder(pSeconField));
            if (iter != listFieldValueResults.end() && (nDiffCount == 0))
            {
                ++nSameCount;
                nLastSameFieldIndex = (*iter)->nIndex;
                pNewFieldValueResult = (*iter)->pFeildResult;
                QString strValue1 = pNewFieldValueResult->leftValue();
                QString strValue2 = pRecord2->asString(i);
                pNewFieldValueResult->setState(strValue1 == strValue2 ? gsStrictlyEqule : gsCompletelyDifferent);
                pNewFieldValueResult->setRightValue(strValue2);
            }
            else
            {
                //从第一个开始没找到的，视为不一样
                ++nDiffCount;
                pNewFieldValueResult = new GCMPFieldValueResult;
                pNewFieldValueResult->setState(gsCompletelyDifferent);
                pNewFieldValueResult->setLeftEmpty(true);
                pNewFieldValueResult->setRightValue(pRecord2->asString(i));
                //插入结果
                GFieldCMPResultPair* pPair = new GFieldCMPResultPair;
                pPair->pFeildResult = pNewFieldValueResult;
                pPair->pField = pSeconField;
                pPair->nIndex = nLastSameFieldIndex != 0 ? nLastSameFieldIndex + 1 : (int)listFieldValueResults.size();
                insertAndFixResultList(listFieldValueResults, nLastSameFieldIndex + 1, pPair);
            }
        }

        oState = nDiffCount == 0 ? gsStrictlyEqule : nSameCount == 0 ? gsCompletelyDifferent : gsPartlyEqule;
        for (auto iter = listFieldValueResults.begin(); iter != listFieldValueResults.end();)
        {
            pResult->addSub((*iter)->pFeildResult);
            delete *iter;
            iter = listFieldValueResults.erase(iter);
        }
        listFieldValueResults.clear();
        pResult->setState(oState);
    }
    return pResult;
}

GGDBEXTEND_EXPORT GCMPTableResult* getTableState(GTable* pTable1, GTable* pTable2)
{
    if (pTable1 == nullptr && pTable2 == nullptr)
    {
        return nullptr;
    }

    GCMPState oState = gsUnknown;
    GCMPTableResult* pTableResult = new GCMPTableResult;
    if (pTable1 == nullptr && pTable2 != nullptr || pTable1 != nullptr && pTable2 == nullptr)
    {
        int nRecordCount = pTable1 == nullptr ? pTable2->recordCount() : pTable1->recordCount();
        bool bSecondNULL = pTable2 == nullptr;
        for (int i = 0 ; i < nRecordCount ; ++i)
        {
            GRecord* pRecord = bSecondNULL ? pTable1->getRecord(i) : pTable2->getRecord(i);
            GCMPRecordResult* pRecordResult = getRecordState(bSecondNULL ? pRecord : nullptr , bSecondNULL ? nullptr : pRecord);
            pTableResult->addSub(pRecordResult);
        }
        pTableResult->setState(gsCompletelyDifferent);
    }
    else
    {
        //都不为空, 表的比较实际上是Record的比较集的父
        //必须使用索引，不然太慢了
        //有索引按索引找， 没索引就按顺序一个个比
        GCMPTableIndexManager oManager;
        if (oManager.isIndexed(pTable1))
        {
            //拿索引找
        }
        else
        {
            //顺序比较
        }
    }
}

GGDBEXTEND_EXPORT GCMPDataBaseResult* getDataBaseState(GDataBase* pDB1, GDataBase* pDB2)
{
    return nullptr;
}

GGDBEXTEND_EXPORT GRecord* searchTargetRecord(GTable* pTargetTable, GRecord* pRefRecord)
{
    //这里要优化
    if (pTargetTable == nullptr || pRefRecord == nullptr)
    {
        return nullptr;
    }

    if (!pRefRecord->isBelongTo(pTargetTable))
    {
        return nullptr;
    }

    //默认用索引找
    int nIndex = pRefRecord->index();
    if (nIndex >= 0 && nIndex < pTargetTable->recordCount())
    {
        GRecord* pTargetRecord = pTargetTable->getRecord(nIndex);
        return pTargetRecord;
    }
}

GGDBEXTEND_EXPORT void getFields(GRecord* pRecord, std::vector<GField*>& vecFields)
{
    if (pRecord == nullptr)
    {
        return ;
    }
    else
    {
        int nFieldCount = pRecord->fieldCount();
        for (int i = 0 ; i < nFieldCount; ++i)
        {
            vecFields.push_back(pRecord->getTable()->getField(i));
        }
    }
}

GGDBEXTEND_EXPORT void getFields(GTable* pTable, std::vector<GField*>& vecFields)
{
    if (pTable == nullptr)
    {
        return ;
    }
    else
    {
        int nFieldCount = pTable->fieldsCount();
        for (int i = 0 ; i < nFieldCount; ++i)
        {
            vecFields.push_back(pTable->getField(i));
        }
    }
}

QString GCMPTableIndexManager::s_strIndexXMLFilePath = "";
GCMPTableIndexManager::~GCMPTableIndexManager()
{

}

GCMPTableIndexManager::GCMPTableIndexManager()
{
    s_strIndexXMLFilePath = exePath().append(strGDBConfigFolder);
}

bool GCMPTableIndexManager::isIndexed(GTable* pTable)
{
    return false;
}

GFieldSchemas GCMPTableIndexManager::getFiledSchemas()
{
    return GFieldSchemas();
}
