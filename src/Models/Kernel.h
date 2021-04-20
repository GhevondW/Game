//
//  Kernel.h
//  Match3Game
//
//  Created by Ghevond Gevorgyan on 19.04.21.
//

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "../GlobalDefines.h"
#include <set>

namespace game {

    class Kernel
    {
    public:
    
        struct Comparator
        {
            bool operator()(const Kernel& k1, const Kernel& k2)
            {
                return k1.GetRank() < k2.GetRank();
            }
        };
    
    public:
    
        Kernel() = default;
        Kernel(const IntMatrix& mat, Element::TYPE reward = Element::TYPE::EMPTY)
            :_mat(mat),
            _reward(reward)
        {
            _row = mat.size();
            _column = mat[0].size();
            for (int y = 0; y < _row; ++y) {
                for (int x = 0; x < _column; ++x) {
                    _rank += mat[y][x];
                }
            }
        }
        Kernel(Kernel&& kernel, Element::TYPE reward = Element::TYPE::EMPTY)
            :_mat(std::move(kernel._mat)),
            _row(kernel._row),
            _column(kernel._column),
            _rank(kernel._rank),
            _reward(reward)
        {}
    
        size_t GetRows() const { return _row; };
        size_t GetCols() const { return _column; };
        size_t GetRank() const { return _rank; };
        Element::TYPE GetRewardType() const { return _reward; }
        
    
        int At(int y, int x) const
        {
            return _mat[y][x];
        }
    
    private:
        IntMatrix       _mat{};
        size_t          _row{};
        size_t          _column{};
        size_t          _rank{};
        Element::TYPE   _reward{Element::TYPE::EMPTY};
    };

    class KernelProvider
    {
    public:
        KernelProvider() = default;
        ~KernelProvider() = default;

        //TODO
        void LoadFromFile() {}
        void AddKernel(const IntMatrix& mat, Element::TYPE reward = Element::TYPE::EMPTY)
        {
            _kernels.emplace(mat, reward);
        }
        void AddKernel(Kernel&& kernel)
        {
            _kernels.emplace(std::move(kernel));
        }
        const std::multiset<Kernel, Kernel::Comparator>& Get() const { return _kernels; }

    private:
        std::multiset<Kernel, Kernel::Comparator> _kernels;
    };
}

#endif /* _KERNEL_H_ */
