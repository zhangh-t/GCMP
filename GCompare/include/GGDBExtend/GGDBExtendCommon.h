#include "GCMPDBExtend.h"
#include "GCMPGGDBResult.h"
#include "GCMPConsts.h"
#include "GGDBExtend/ggdbextend_global.h"


struct GFieldCMPResultPair
{
    GField* pField;
    GCMPFieldValueResult* pFeildResult;
    int nIndex;
};

struct GRecordCMPResultPair
{
    GRecord* pRecord;
    GCMPRecordResult* pRecordResult;
    int nIndex;
};


struct GFieldFinder
{
    GFieldFinder(GField* pField) : m_pField(pField){}
    bool operator() (GFieldCMPResultPair* pOther)
    {
        return (*(m_pField->schema())) == (*(pOther->pField->schema()));
    }
private:
    GField* m_pField;
};

struct GRecordFinder
{
    GRecordFinder(GRecord* pTargetRecord) : m_pTargetRecord(pTargetRecord) {}
    bool operator() (GRecord* pOtherRecord)
    {
        
    }

private:
    GRecord* m_pTargetRecord;
};

class GGDBEXTEND_EXPORT GCMPTableIndexManager
{
public:
    GCMPTableIndexManager();
    virtual ~GCMPTableIndexManager();

public:
    bool isIndexed(GTable* pTable);
    GFieldSchemas getFiledSchemas();
private:

private:
    static QString s_strIndexXMLFilePath;

};

GGDBEXTEND_EXPORT void getFields(GRecord* pRecord, std::vector<GField*>& vecFields);
GGDBEXTEND_EXPORT void getFields(GTable* pTable, std::vector<GField*>& vecFields);

//在pTargetTable里去找和pRefRecord最像的记录
GGDBEXTEND_EXPORT GRecord* searchTargetRecord(GTable* pTargetTable, GRecord* pRefRecord);

//一下返回的结果都需要释放
//判断两个字段的状态，只有严格相等或完全不一样两种状态
GGDBEXTEND_EXPORT GCMPFieldResult* getDBFieldState(GField* pField1, GField* pField2);

//判断两条记录的状态
GGDBEXTEND_EXPORT GCMPRecordResult* getRecordState(GRecord* pRecord1, GRecord* pRecord2);

//判断两个表格的状态
GGDBEXTEND_EXPORT GCMPTableResult* getTableState(GTable* pTable1, GTable* pTable2);

//判断两个数据库的状态
GGDBEXTEND_EXPORT GCMPDataBaseResult* getDataBaseState(GDataBase* pDB1, GDataBase* pDB2);
