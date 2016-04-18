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

//��pTargetTable��ȥ�Һ�pRefRecord����ļ�¼
GGDBEXTEND_EXPORT GRecord* searchTargetRecord(GTable* pTargetTable, GRecord* pRefRecord);

//һ�·��صĽ������Ҫ�ͷ�
//�ж������ֶε�״̬��ֻ���ϸ���Ȼ���ȫ��һ������״̬
GGDBEXTEND_EXPORT GCMPFieldResult* getDBFieldState(GField* pField1, GField* pField2);

//�ж�������¼��״̬
GGDBEXTEND_EXPORT GCMPRecordResult* getRecordState(GRecord* pRecord1, GRecord* pRecord2);

//�ж���������״̬
GGDBEXTEND_EXPORT GCMPTableResult* getTableState(GTable* pTable1, GTable* pTable2);

//�ж��������ݿ��״̬
GGDBEXTEND_EXPORT GCMPDataBaseResult* getDataBaseState(GDataBase* pDB1, GDataBase* pDB2);
