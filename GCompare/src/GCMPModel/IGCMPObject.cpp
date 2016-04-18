#include "GCMPModel/IGCMPObject.h"


IGCMPObject::IGCMPObject(void)
{
}


IGCMPObject::~IGCMPObject(void)
{
}

IGCMPResult::~IGCMPResult()
{
    for (auto iter = m_vecSubReulst.begin(); iter != m_vecSubReulst.end(); ++iter)
    {
        if (*iter != nullptr)
        {
            delete *iter;
        }
    }
    m_vecSubReulst.clear();
}
