#include "ClassInfo.h"

#include <algorithm>


ClassInfo* ClassInfo::First = nullptr;


void ClassInfo_Cleanup(ClassInfo* InClass)
{
    auto it = InClass->Properties.begin();
    while (it != InClass->Properties.end())
    {
        auto ptr = (*it);
        ++it;
        
        delete ptr;
    }
}


bool ClassInfo::IsAClass(const ClassInfo* InBaseClass)
{
    ClassInfo* Temp = BaseClass;
    while (Temp)
    {
        if (Temp == InBaseClass)
        {
            return true;
        }
        // else
        Temp = Temp->BaseClass;
    }

    return false;
}



void ClassInfo::BuildupReplicationProps()
{
    if (HasAnyFlags(ClassInfoFlag_InitReplicationData))
    {
        return;
    }
    
    if (BaseClass)
    {
        BaseClass->BuildupReplicationProps();

        ClassReps = BaseClass->ClassReps;
    }
    else
    {
        ClassReps.clear();
    }
    
    std::vector<CProperty*> NetProperty;
    for (auto it = Properties.begin(); it != Properties.end(); ++it)
    {
        if ((*it)->Flag & PropertyFlag_Net)
        {
            // 只记录下需要进行网络复制的属性
            NetProperty.push_back(*it);
        }
    }

    do
    {
        size_t NewSize = ClassReps.size() + NetProperty.size();
        if (NewSize == 0)
        {
            break;
        }

        ClassReps.reserve(NewSize);
        
        struct SortNetPropertes
        {
            inline bool operator()(const CProperty* l, const CProperty* r) const
            {
                if (l->Offset == r->Offset)
                {
                    return strcmp(l->Name, r->Name) <= 0;
                }
                else
                {
                    return l->Offset < r->Offset;
                }
            }
        };
        std::sort(NetProperty.begin(), NetProperty.end(), SortNetPropertes());

        for (size_t i=0; i<NetProperty.size(); ++i)
        {
            CProperty* pro = NetProperty[i];
            pro->RepIndex = (int)ClassReps.size();

            for (int j=0; j<pro->ArrayDim; ++j)
            {
                ClassReps.emplace_back(FRepRecord(pro, j));
            }
        }
    }
    while (0);

    ClassFlag |= ClassInfoFlag_InitReplicationData;
}


int ClassInfo::GetOffset(const char* FieldName) const
{
    for (auto it=Properties.begin(); it!=Properties.end(); ++it)
    {
        if (strcmp(FieldName, (*it)->Name) == 0)
        {
            return (*it)->Offset;
        }
    }
    
    return -1;
}

