#ifndef __PROCESSBROWSERMODEL__
#define __PROCESSBROWSERMODEL__

#include <vector>
#include "Unii/Unii.h"
#include "Data/ProcessItem.h"

namespace Dnject
{
    class ProcessBrowserModel : public Unii::IModel
    {
        private:
            std::vector<ProcessItem> m_processes;

        public:
            std::vector<ProcessItem>& Processes()
            {
                return m_processes;
            }

            const std::vector<ProcessItem>& Processes() const
            {
                return m_processes;
            }
    };
}

#endif
