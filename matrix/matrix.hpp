#pragma once
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <iterator>
#include <stdexcept>

namespace matrix{

template<typename T>
class matrix_t
{
    int height = 0, width = 0;
    T* data = nullptr;

    int* row_order = nullptr;
    int* col_order = nullptr;

    void init_orders()
    {
        for (auto i = 0; i < height; i++)
            row_order[i] = i;
        for (auto i = 0; i < width; i++)
            col_order[i] = i;
    }

    public:
    matrix_t(int h, int w, T val = T{})
    :height {h}, width {w}, data {new T[height * width]},
     row_order {new int[height]}, col_order {new int[width]}
    {
        if (height < 0 || width < 0)
            throw std::length_error{"In constructor of matrix: height or width less null"};

        for (auto i = 0; i < height * width; i++)
            data[i] = val;
        init_orders();
    }

    template<std::input_iterator it>
    matrix_t(int h, int w, it begin, it end)
    :height {h}, width {w}, data {new T[height * width]},
     row_order {new int[height]}, col_order {new int[width]}
    {
        if (height < 0 || width < 0)
            throw std::length_error{"In constructor of matrix: height or width less null"};
        
        for (auto i = 0, itr = begin; i < height * width || itr != end; i++, ++itr)
            data[i] = *itr;
        init_orders();    
    }

    matrix_t(T val = T{})
    :height {1}, width {1}, data {new T[1]{val}},
     row_order {new int[1]{0}}, col_order {new int[1]{0}} {}

    matrix_t(std::initializer_list<T> onedim_list)
    :height {onedim_list.size()}, width {1}, data {new T[height * width]},
     row_order {new int[height]}, col_order {new int[width]}
    {
        auto i = 0;
        for (auto elem: onedim_list)
            data[i++] = elem;
        init_orders();
    }

    matrix_t(std::initializer_list<std::initializer_list<T>> twodim_list)
    :height {twodim_list.size()}
    {
        if (height == 0)
            throw std::logic_error{"Never know why - Ozzy Osbourne"};
        
        std::size_t max_width = 0;
        for (auto row: twodim_list)
            if (row.size() > max_width)
                max_width = row.size();

        width = max_width;
        data = new T[height * width];

        std::size_t i = 0, j = 0;
        for (auto row: twodim_list)
        {
            for (auto elem: row)
                data[i * width + j++] = elem;
            i++;
        }
    }

    matrix_t(const matrix_t& rhs)
    :height {rhs.height}, width {rhs.width}, data {new T[height * width]},
     row_order {new int[height]}, col_order {new int[width]}
    {
        std::copy(rhs.data, rhs.data + height * width, data);
        std::copy(rhs.row_order, rhs.row_order + height, row_order);
        std::copy(rhs.col_order, rhs.col_order + width, col_order);
    }

    matrix_t(matrix_t&& rhs)
    :height {rhs.height}, width {rhs.width}, data {rhs.data},
     row_order (rhs.row_order), col_order {rhs.col_order}
    {}

    matrix_t& operator=(const matrix_t& rhs)
    {
        height = rhs.height;
        width  = rhs.width;
        data = new T*[height]{new T[width] {}};

        for (auto i = 0; i < height; i++)
            for (auto j = 0; j < width; j++)
                rhs.data[i][j] = data[i][j];
        return *this;
    }

    matrix_t& operator=(matrix_t&& rhs)
    {
        if (this == &rhs)
            return *this;

        height = rhs.height;
        width  = rhs.width;
        data   = rhs.data;
        return *this;
    }

    ~matrix_t()
    {
        for (auto i = 0; i < height; i++)
            delete[] data[i];
        delete[] data;
    }

    static matrix_t quad(int sz, T val = T{})
    {
        return matrix_t{sz, sz, val};
    }

    template<std::input_iterator it>
    static matrix_t quad(int sz, it begin, it end)
    {
        return matrix_t{sz, sz, begin, end};
    }

    template<std::input_iterator it>
    static matrix_t diag(int sz, it begin, it end)
    {
        matrix_t result {quad(sz)};
        for (auto i = 0, itr = begin; i < sz && itr != end; ++itr, i++)
            result.data[i][i] = *itr;
        return result;
    }

    template<std::input_iterator it>
    static matrix_t diag(it begin, it end)
    {
        auto sz = 0;
        for (auto itr = begin; itr != end; ++itr, sz++) {;}
        return diag(sz, begin, end);
    }

    static matrix_t diag(int sz, T val = T{})
    {
        matrix_t result {quad(sz)};
        for (auto i = 0; i < sz; i++)
            result.data[i][i] = val;
        return result;
    }

    std::ostream& dump(std::ostream& out) const 
    {
        out << '{';
        for (auto i = 0; i < height; i++)
        {
            out << '{';
            if (width != 0)
            {
                for (auto j = 0; j < width - 1; j++)
                    out << data[i][j] << ' ';
                out << data[i][width - 1];
            }
            out << '}';
        }
        out << '}';
        return out;
    }    
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const matrix_t<T>& mat)
{
    return mat.dump(out);
}

} //namespace
