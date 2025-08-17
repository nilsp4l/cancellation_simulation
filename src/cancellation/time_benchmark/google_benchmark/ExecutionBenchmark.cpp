#include "benchmark/benchmark.h"
#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/tree/Builder.hpp"


template<cancellation::CancelType cancel_type, cancellation::CleanupType cleanup_type>
class ExecutionFixture : public benchmark::Fixture {
public:
    void SetUp(const benchmark::State &) override {
        cancel_checkpoint_registry_ = std::make_unique<cancellation::time_benchmark::CancelCheckpointRegistry>();
        context_ = std::make_unique<cancellation::query::Context<cancel_type, cleanup_type> >(
            cancel_checkpoint_registry_.get());

        root_ = cancellation::tree::Builder<cancel_type, cleanup_type, 10'000'000, 10>::build(
            context_.get(), cancel_checkpoint_registry_.get());
    }

    std::unique_ptr<cancellation::tree::Node> root_;


    //ignored but needs to hold a value
    std::unique_ptr<cancellation::time_benchmark::CancelCheckpointRegistry> cancel_checkpoint_registry_;

    std::unique_ptr<cancellation::query::Context<cancel_type, cleanup_type> > context_;
};


BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_exchg_er,
                     cancellation::CancelType::kFunctionExchg,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_exchg_ex,
                     cancellation::CancelType::kFunctionExchg,
                     cancellation::CleanupType::kException)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_ptr_exchg_er,
                     cancellation::CancelType::kFunctionPointerExchg,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_ptr_exchg_ex,
                     cancellation::CancelType::kFunctionPointerExchgJustExec,
                     cancellation::CleanupType::kException)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     atom_er,
                     cancellation::CancelType::kAtomicEnum,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     atom_ex,
                     cancellation::CancelType::kAtomicEnum,
                     cancellation::CleanupType::kException)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_ptr_exchg_comp_er,
                     cancellation::CancelType::kFunctionPointerExchgDifferentCompUnit,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_ptr_exchg_comp_ex,
                     cancellation::CancelType::kFunctionPointerExchgDifferentCompUnit,
                     cancellation::CleanupType::kException)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}


BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     fct_ptr_exchg_call_er,
                     cancellation::CancelType::kFunctionPointerExchgCallConv,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     interval_er,
                     cancellation::CancelType::kInterval,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     interval_ex,
                     cancellation::CancelType::kInterval,
                     cancellation::CleanupType::kException)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     union_er,
                     cancellation::CancelType::kUnion,
                     cancellation::CleanupType::kErrorReturn)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}

BENCHMARK_TEMPLATE_F(ExecutionFixture,
                     union_ex,
                     cancellation::CancelType::kUnion,
                     cancellation::CleanupType::kException)(benchmark::State &st) {
    for (auto _: st) {
        benchmark::DoNotOptimize(root_->next());
    }
}


BENCHMARK_MAIN();
