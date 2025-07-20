#pragma once

#include "cancellation/tree/LeafNode.hpp"
#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/query/Context.hpp"

namespace cancellation::tree {
    /*
    template<CancelType, CleanupType>
    class LeafNodeImpl : public Node {
    };
    */

    template<CancelType cancel_type, CleanupType cleanup_type>
    class LeafNodeImpl : public Node {
    public:
        explicit LeafNodeImpl(std::size_t delay_count,
                              query::Context<cancel_type, cleanup_type> *
                              context,
                              benchmark::CancelCheckpointRegistry *
                              cancel_checkpoint_registry) : cancel_checkpoint_registry_(cancel_checkpoint_registry),
                                                            delay_count_(delay_count), context_(context) {
        }


        int next() override {
            if constexpr (cleanup_type == CleanupType::kErrorReturn) {
                if (auto error = context_->checkForInterrupt(); static_cast<bool>(error)) {
                    cancel_checkpoint_registry_->registerCheckpoint(benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                    return static_cast<int>(error);
                }
            } else {
                context_->checkForInterrupt(); // throws
            }

            volatile std::size_t inner_delay{inner_delay_amount};

            while (--inner_delay);

            if (--delay_count_ > 0) {
                return static_cast<int>(util::Error::kSuccess);
            }

            // we are done
            return static_cast<int>(util::Error::kFinished);
        }

    private:
        volatile std::size_t delay_count_;
        query::Context<cancel_type, cleanup_type> *context_;
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
        static constexpr std::size_t inner_delay_amount{1000};
    };
}
