#include "GGDBExtend/GCMPDBExtend.h"
#include "GGDBExtend/GCMPGGDBResult.h"
#include "GGDBExtend/GGDBExtendCommon.h"

GDataBase::GDataBase(CDatabase* pDataBase)
{
    setType(gotDataBase);
    m_pDataBase = pDataBase;
    if (m_pDataBase != nullptr)
    {
        int nTableCount = pDataBase->TableCount();
        for (int i = 0 ; i < nTableCount; ++i)
        {
            //CDBTable的内存由CDataBase维护
            CDBTable* pDBTable = pDataBase->GetTable(i);
            //GTable的内存有GDataBase维护
            GTable* pGTable = new GTable(pDBTable);
            m_listTables.push_back(pGTable);
        }
    }
}

GDataBase::~GDataBase(void)
{
    for (auto iter = m_listTables.begin(); iter != m_listTables.end();
        ++iter)
    {
        delete *iter;
    }

    if (m_pDataBase != nullptr)
    {
        m_pDataBase->Close();
        delete m_pDataBase;
        m_pDataBase = nullptr;
    }
}

GTable* GDataBase::getTable(int nIndex)
{
    if (nIndex < (int)m_listTables.size() && nIndex >= 0)
    {
        return m_listTables.at(nIndex);
    }
    else
    {
        return nullptr;
    }
}

bool GDataBase::close()
{
    m_pDataBase->Close();
    return true;
}

IGCMPResult* GDataBase::compare(IGCMPObject* pOther)
{
    return nullptr;
}

bool GDataBase::canCompare(IGCMPObject* pOther)
{
    return false;
}

GTable::GTable(CDBTable* pTable)
{
    setType(gotTable);
    m_pTable = pTable;
    if (m_pTable != nullptr)
    {
        m_strTableName = QString::fromWCharArray(m_pTable->Name());
        int nFieldCount = m_pTable->FieldCount();
        for (int i = 0 ; i < nFieldCount - 1 ; ++i)
        {
            //第0列是一个空列
            CDBField* pDBField = m_pTable->GetField(i + 1);
            GField* pField = new GField(pDBField);
            m_listFields.push_back(pField);
        }

        int nRecordCount = m_pTable->RecordCount();
        CFileAddressList* pAddressList = new CFileAddressList();
        m_pTable->GetAllRecords(pAddressList);
        for (int i = 0 ; i < nRecordCount; ++i)
        {
            FileAddress* pFileAddredd = pAddressList->GetItem(i);
            CDBRecord* pDBRecord = m_pTable->CreateRecordMap(*pFileAddredd);
            //内存由GRecord接管
            GRecord* pGRecord = new GRecord(pDBRecord, this, i);
            m_listRecords.push_back(pGRecord);
        }
        delete pAddressList;
    }

    m_pSchema = new GTableSchema(this);
}

GTable::~GTable()
{
    for (auto iter = m_listFields.begin(); iter != m_listFields.end(); ++iter)
    {
        delete *iter;
    }

    for(auto iter = m_listRecords.begin(); iter != m_listRecords.end(); ++iter)
    {
        delete* iter;
    }

    delete m_pSchema; 
    //析构的时候不用析构CDBTable
    //CDBTable由CDataBase析构
    //CDataBase由GDataBase析构
}

GField* GTable::getField(int nIndex)
{
    if (nIndex < (int)m_listFields.size() && nIndex >= 0)
    {
        return m_listFields.at(nIndex);
    }
    else
    {
        return nullptr;
    }
}

GRecord* GTable::getRecord(int nIndex)
{
    if (nIndex < (int)m_listRecords.size())
    {
        return m_listRecords.at(nIndex);
    }
    else
    {
        return nullptr;
    }
}

IGCMPResult* GTable::compare(IGCMPObject* pOther)
{
    //表比的是记录
    IGCMPResult* pRet = nullptr;
    if (canCompare(pOther))
    {
        GTable* pOtherTable = dynamic_cast<GTable*>(pOther);
        pRet = getTableState(this, pOtherTable);
    }
    return pRet;
}

bool GTable::canCompare(IGCMPObject* pOther)
{
    //表是否可以比较
    //表名一样就能比较
    GTable* pOtherTable = dynamic_cast<GTable*>(pOther);
    if (pOtherTable != nullptr)
    {
        return (type() == pOther->type()) && (tableName() == pOtherTable->tableName());
    }
    else
    {
        return false;
    }
    
}

GField::~GField()
{

}

IGCMPResult* GField::compare(IGCMPObject* pOther)
{
   IGCMPResult* pRetResult = nullptr;
   if (canCompare(pOther))
   {
       GCMPFieldResult* pFieldResult = nullptr;
       GField* pOtherField = dynamic_cast<GField*>(pOther);
       if (pOtherField != nullptr)
       {
           pFieldResult = getDBFieldState(this, pOtherField);
           pRetResult = pFieldResult;
       }
   }
   return pRetResult;
}

bool GField::canCompare(IGCMPObject* pOther)
{
    return type() == pOther->type();
}

GField::GField(CDBField* pField)
{
    setType(gotField);
    m_pDBField = pField;
    if (m_pDBField != nullptr)
    {
        m_strFieldName = QString::fromStdWString(pField->Name());
        m_nIndex = pField->Index();
        m_pSchema = new GFieldSchema(m_strFieldName, m_pDBField->Type());
    }
}

GRecord::~GRecord()
{

}

GRecord::GRecord(CDBRecord* pRecord, GTable* pTable, int nIndex)
{
    setType(gotRecord);
    m_pDBRecord = pRecord;
    m_pDBTable = pTable;
    m_nIndex = nIndex;
}

bool GRecord::isNull(int nFieldIndex)
{
    CDBField* pDBField = getField(nFieldIndex);
    if (pDBField != nullptr)
    {
        return pDBField->IsDataNull(m_pDBRecord);
    }
    else
    {
        assert(pDBField != nullptr);
        return false;
    }
}

bool GRecord::asBoolean(int nFieldIndex)
{
    CDBField* pDBField = getField(nFieldIndex);
    if (pDBField != nullptr)
    {
        return pDBField->GetBool(m_pDBRecord);
    }
    else
    {
        return false;
    }
}


int GRecord::asInteger(int nFieldIndex)
{
    CDBField* pDBField = getField(nFieldIndex);
    if (pDBField != nullptr)
    {
        return pDBField->GetInteger(m_pDBRecord );
    }
    else
    {
        assert(false);
        return 0;
    }
}


__int64 GRecord::asInteger64(int nFieldIndex)
{
    CDBField* pDBField = getField(nFieldIndex);
    if (pDBField != nullptr)
    {
        return pDBField->GetInt64(m_pDBRecord );
    }
    else
    {
        assert(false);
        return 0;
    }
}

double GRecord::asDouble(int nFieldIndex)
{
    CDBField* pDBField = getField(nFieldIndex);
    if (pDBField != nullptr)
    {
        return pDBField->GetDouble(m_pDBRecord );
    }
    else
    {
        assert(false);
        return 0.0;
    }
}

QString GRecord::asString(int nFieldIndex)
{
    CDBField* pDBField = getField(nFieldIndex);
    if (pDBField != nullptr)
    {
        if (pDBField->DataType(m_pDBRecord ) == FIELDTYPE_DOUBLE)
        {
            wchar_t pStr[128] = {0};
            swprintf_s(pStr, 128, L"%.15g", pDBField->GetDouble(m_pDBRecord ));
            return QString::fromWCharArray(pStr);
        }
        else
        {
            assert(pDBField->DataType(m_pDBRecord ) != FIELDTYPE_STREAM);
            ggp::CString* pCString = pDBField->CreateStringMap(m_pDBRecord );
            if (pCString != nullptr)
            {
                wchar_t* buffer_str = pCString->Buffer();
                delete pCString;
                return QString::fromWCharArray(buffer_str);
            }
            else
            {
                return "";
            }
        }
    }
    else
    {
        assert(false);
        return "";
    }
}

CDBField* GRecord::getField(int nFieldIndex)
{
    if (m_pDBTable != nullptr && m_pDBTable->dbTable() != nullptr)
    {
        return m_pDBTable->dbTable()->GetField(nFieldIndex);
    }
    else
    {
        return nullptr;
    }
}

IGCMPResult* GRecord::compare(IGCMPObject* pOther)
{
    IGCMPResult* pRet = nullptr;
    if (canCompare(pOther))
    {
        GRecord* pRecord = dynamic_cast<GRecord*>(pOther);
        if (pRecord != nullptr)
        {
            pRet = getRecordState(this, pRecord);
        }
    }
    return pRet;
}

bool GRecord::canCompare(IGCMPObject* pOther)
{
    //表能比较，记录就能比较
    bool bRet = false;
    if (pOther != nullptr)
    {
        if (pOther->type() == gotRecord)
        {
            GRecord* pOtherRecord = dynamic_cast<GRecord*>(pOther);
            if (pOtherRecord)
            {
                if (this->getTable()->canCompare(pOtherRecord->getTable()))
                {
                    bRet = true;
                }
            }
        }
    }
    return bRet;
}

int GRecord::fieldCount() const
{
    if (m_pDBTable != nullptr && m_pDBTable->dbTable())
    {
        return m_pDBTable->dbTable()->FieldCount();
    }
    else
    {
        return -1;
    }
}

QString GRecord::getFieldName(const int nIndex)
{
    if (m_pDBTable != nullptr)
    {
        CDBField* pField = getField(nIndex);
        if (pField != nullptr)
        {
            return QString::fromStdWString(pField->Name());
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}

bool GRecord::isBelongTo(GTable* pTable)
{
    if (pTable == nullptr)
    {
        return false;
    }
    else
    {
        //判断字段类是否完全一致
        std::vector<GField*> vecField1;
        std::vector<GField*> vecField2;
        getFields(pTable, vecField1);
        getFields(this, vecField2);
        if (vecField1.size() != vecField2.size())
        {
            return false;
        }
        for (int i = 0 ; i < vecField1.size(); ++i)
        {
            GField* pField1 = vecField1.at(i);
            GField* pField2 = vecField2.at(i);
            if (!((*pField1->schema()) == (*pField2->schema())))
            {
                return false;
            }
        }
    }
    return true;
}

GTableSchema::GTableSchema(GTable* pTable)
{
    int nFieldCount = pTable->fieldsCount();
    for (int i = 0; i < nFieldCount; ++i)
    {
        GField* pFeild = pTable->getField(i);
        GFieldSchema* pFeildSchema = new GFieldSchema(pFeild->fieldName(), pFeild->getFieldType());
        m_vecFieldSchemas.push_back(pFeildSchema);
    }
}

GTableSchema::~GTableSchema()
{
    for (auto iter = m_vecFieldSchemas.begin(); iter != m_vecFieldSchemas.end();)
    {
        delete *iter;
        iter = m_vecFieldSchemas.erase(iter);
    }
    m_vecFieldSchemas.clear();
}

bool GTableSchema::operator==(GTableSchema pOther)
{
    //每一个fieldSchema都一样
    bool bRet = false;
    if (pOther.fieldSchemaCount() == fieldSchemaCount())
    {
        int nCount = fieldSchemaCount();
        for (int i = 0; i < nCount; ++i)
        {
            GFieldSchema* pOtherSchema = pOther.fieldSchema(i);
            if (std::find(m_vecFieldSchemas.begin(), m_vecFieldSchemas.end(), pOtherSchema) 
                == m_vecFieldSchemas.end())
            {
                break;
            }
        }
        bRet = true;
    }
    return bRet;
}

GFieldSchema* GTableSchema::fieldSchema(int nFeildIndex)
{
    if (nFeildIndex >= 0 && nFeildIndex < m_vecFieldSchemas.size())
    {
        return m_vecFieldSchemas.at(nFeildIndex);
    }
    else
    {
        return nullptr;
    }
}
