#ifndef __UNII_IPRESENTER__
#define __UNII_IPRESENTER__

namespace Unii
{
    class IPresenter
    {
        public:
            virtual ~IPresenter() = default;
            virtual void Initialize() = 0;
            virtual void Update() = 0;
            virtual void Shutdown() = 0;
    };
}

#endif
