#pragma once

#include <CTRPluginFramework.hpp>
#include "RuntimeContext.hpp"

namespace CTRPluginFramework {
    class RuntimeContext {
        private:
            bool v_isTurbo = false;
            bool v_isFov = false;
            bool v_isSaveMenuDisabled = false;
            bool v_isIndoors = false;

            RuntimeContext() = default;
            static RuntimeContext* instance;
        public:
            static RuntimeContext* getInstance();

            bool isTurbo() const;
            void setTurbo(bool value);
            bool isFov() const;
            void setFov(bool value);
            bool isSaveMenuDisabled() const;
            void setSaveMenuDisabled(bool value);
            bool isIndoors() const;
            void setIndoors(bool value);
    };
}