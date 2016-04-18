#pragma once
#include "ggdbextend_global.h"
#include "GGDB/ggdbdbmgr.h"
#include "GGDB/ggdbdef.h"
#include "GCMPModel/IGCMPObject.h"
#include <vector>
#include <QString>
using namespace ggp;


class GGDBEXTEND_EXPORT GFieldSchema
{
public:
    GFieldSchema(QString strFieldName, FieldType oType) 
        : m_strFieldName(strFieldName), m_oType(oType) {}
    virtual ~GFieldSchema() {}

    bool operator == (const GFieldSchema& other)
    {
        return (fieldName() == other.fieldName()) && 
            (fieldType() == other.fieldType());
    }

public:
    QString fieldName() const {return m_strFieldName;}
    FieldType fieldType() const {return m_oType;}

private:
    QString m_strFieldName;
    FieldType m_oType;
};

typedef std::vector<GFieldSchema*> GFieldSchemas;

class GTable;
class GGDBEXTEND_EXPORT GTableSchema
{
    //表格的schema
public:
    GTableSchema(GTable* pTable);
    virtual ~GTableSchema();

public:
    bool operator == (GTableSchema pOther);

    GFieldSchema* fieldSchema(int nFeildIndex);

    int fieldSchemaCount() { return m_vecFieldSchemas.size();}
private:
    std::vector<GFieldSchema*> m_vecFieldSchemas;
};

class GGDBEXTEND_EXPORT GField : public IGCMPObject
{
public:
    GField(CDBField* pField);
    virtual ~GField();
    
    GFieldSchema* schema() {return m_pSchema;}

    QString fieldName() const {return m_strFieldName;}

    int getIndexInable() const {return m_nIndex;}

    FieldType getFieldType() const {return m_pDBField->Type();}

    virtual IGCMPResult* compare(IGCMPObject* pOther);

    virtual bool canCompare(IGCMPObject* pOther);
private:
    QString m_strFieldName;
    CDBField* m_pDBField;
    int m_nIndex;
    GFieldSchema* m_pSchema;
};

class GTable;
//只读
class GGDBEXTEND_EXPORT GRecord : public IGCMPObject
{
public:
    GRecord(CDBRecord* pRecord, GTable* pTable, int nIndex);
    virtual ~GRecord();

    virtual IGCMPResult* compare(IGCMPObject* pOther);

public:
    bool isBelongTo(GTable* pTable);

    bool isNull(int nFieldIndex);

    bool asBoolean(int nFieldIndex);

    int asInteger(int nFieldIndex);

    __int64 asInteger64(int nFieldIndex);

    double asDouble(int nFieldIndex);

    QString asString(int nFieldIndex);

    int fieldCount() const;

    QString getFieldName(const int nIndex);

    GTable* getTable() {return m_pDBTable;}

    int index() const {return m_nIndex;}
protected:
    virtual bool canCompare(IGCMPObject* pOther);

private:
    CDBField* getField(int nFieldIndex);

private:
    CDBRecord* m_pDBRecord;
    GTable* m_pDBTable;
    int m_nIndex;   //在表里的索引
};

class GGDBEXTEND_EXPORT GTable : public IGCMPObject
{
    friend class GRecord;
public:
    GTable(CDBTable* pTable);
    virtual ~GTable();

    QString tableName() const {return m_strTableName;}

    int recordCount() const {return (int)m_listRecords.size();}

    GRecord* getRecord(int nIndex);

    int fieldsCount() const {return (int)m_listFields.size();}

    GField* getField(int nIndex);

    virtual IGCMPResult* compare(IGCMPObject* pOther);

    GTableSchema* tableSchema() { return m_pSchema; }
protected:
    virtual bool canCompare(IGCMPObject* pOther);

private: 
    CDBTable* dbTable() {return m_pTable;}
private:
    std::vector<GField*> m_listFields;
    std::vector<GRecord*> m_listRecords;
    QString m_strTableName;
    CDBTable* m_pTable;
    GTableSchema* m_pSchema;
};

class GGDBEXTEND_EXPORT GDataBase : public IGCMPObject
{
public:
    GDataBase(CDatabase* pDataBase);
    ~GDataBase(void);

    GTable* getTable(int nIndex);

    bool close();

    int tableCount() const {return (int)m_listTables.size();}

    void flush() {if (m_pDataBase != nullptr){m_pDataBase->Flush();}}

    IGCMPResult* compare(IGCMPObject* pOther);

protected:
    virtual bool canCompare(IGCMPObject* pOther);

private:


private:
    std::vector<GTable*> m_listTables;
    CDatabase* m_pDataBase;
};