#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
    using KeyVector = std::vector<Key>;

    class   KeySequence {
        public:

            KeySequence(KeyVector sequence);
            ~KeySequence(){}

            /**
             * \brief Check the sequence
             * \return true if the sequence is completed, false otherwise
             */
            bool  operator()(void);

        private:

            KeyVector   _sequence;
            Clock       _timer;
            int         _indexInSequence;
    };
}