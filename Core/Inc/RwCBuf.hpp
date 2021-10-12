/**
 * @file RwCBuf.hpp
 * @author Eduard Ziganshin (zig.edu@gmail.com)
 * @brief Rewritable FIFO Ring Buffer
 * @note CPP11, STL Free, noexcept
 * @version 1.0
 * @date 12-01-2021
 */

#pragma once

#include <stddef.h>

/**
 * @brief Rewritable FIFO Ring Buffer Template Class
 * @note There are two operation modes: Rewrite mode and Non-Rewrite mode. @n
 * Mode is set by a constructor parameter or by a parameter of the Reset()
 * method. @n In Rewrite mode the fill flag is not set.
 * @param[in] SIZE - buffer size
 * @param[in] T - type of buffer items
 */

template<size_t SIZE, typename T>
class RwCBuf {
  static_assert(SIZE > 1, "RwCBuf::SIZE must be at least 2");

 public:
  using value_type = T;              ///< Type of buffer items
  using size_type = decltype(SIZE);  ///< Array index type

  static constexpr size_type size = SIZE;

  enum class rwType : bool  ///< Buffer operation mode
  {
    nonRwBuf = false,  ///< Non-Rewrite mode
    rwBuf = true       ///< Rewrite mode
  };

 private:
  /**
   * @brief Ring Buffer Index Template Class
   * @note Implemented as a wrapper over size_type @n
   * Default constructor. Other constructors and @n
   * assignment operators are prohibited.
   * @param[in] INDEX_SIZE - buffer size
   */

  template<size_type INDEX_SIZE>
  class RwCBufIndex {
   public:
    RwCBufIndex() = default;
    RwCBufIndex(const RwCBufIndex&) = delete;
    RwCBufIndex(const RwCBufIndex&&) = delete;
    RwCBufIndex& operator=(const RwCBufIndex&) = delete;
    RwCBufIndex& operator=(const RwCBufIndex&&) = delete;

    /**
     * @brief Pre-increment operator, return a copy of the index value
     */
    size_type operator++() noexcept {
      i = size_type((i + 1) % INDEX_SIZE);
      return i;
    }
    ;

    /**
     * @brief Post-increment operator, return a copy of the index value
     */
    size_type operator++(int) noexcept {
      size_type ret { i };
      i = size_type((i + 1) % INDEX_SIZE);
      return ret;
    }
    ;

    /**
     * @brief Operator (), returns a copy of the current index value
     */
    size_type operator()() noexcept {
      return i;
    }

    /**
     * @brief Reset the value of index to zero
     */
    void reset() noexcept {
      i = 0;
    }

   private:
    size_type i { };  ///< Index value
  };

  value_type _data[SIZE] { };  ///< Buffer Array

  RwCBufIndex<SIZE> _begin;  ///< Data start index
  RwCBufIndex<SIZE> _end;    ///< Data end index

  rwType _reWriteMode;  ///< Operation mode

  bool _isOverwritten { };  ///< An indication that an overwrite has occurred
  bool _isEmpty { true };    ///< Empty buffer flag
  bool _isFull { };         ///< Full buffer flag

 public:
  /**
   * @brief Constructor with operation mode setting
   * @param[in] RW - operation mode (default - Rewrite mode)
   */
  explicit RwCBuf(rwType RW = rwType::rwBuf) noexcept
      :
      _reWriteMode { RW } {
  }

  RwCBuf(const RwCBuf&) = delete;
  RwCBuf(const RwCBuf&&) = delete;
  RwCBuf& operator=(const RwCBuf&) = delete;
  RwCBuf& operator=(const RwCBuf&&) = delete;

  /**
   * @brief Clear buffer and set operation mode
   * @param[in] rwType - operation mode
   */
  void reset(rwType RW) noexcept {
    _reWriteMode = RW;
    clear();
  }

  /**
   * @brief Clear buffer
   */
  void clear() noexcept {
    _begin.reset();
    _end.reset();
    _isOverwritten = false;
    _isEmpty = true;
    _isFull = false;
  }

  /**
   * @brief Current operation mode
   * @return rwType - operation mode
   */
  rwType reWriteMode() noexcept {
    return _reWriteMode;
  }

  /**
   * @brief Returns the flag of the overwrite that occurred @n
   * in Rewrite mode. In Non-Rewrite mode flag always clear
   * @note After reading, the flag is cleared
   * @return An indication that an overwrite has occurred
   */
  bool isOverwritten() noexcept {
    bool res = _isOverwritten;
    _isOverwritten = false;
    return res;
  }

  /**
   * @brief Returns the full buffer flag
   * @note In Rewrite mode this flag always clear
   * @return bool - Full buffer flag
   */
  bool isFull() noexcept {
    return _isFull;
  }

  /**
   * @brief Returns the empty buffer flag
   * @return bool - Empty buffer flag
   */
  bool isEmpty() noexcept {
    return _isEmpty;
  }

  /**
   * @brief Writing a value to a buffer
   * @note Returns the result of writing @n
   * An unsuccessful write is possible only with a buffer overflow @n
   * in Non-Rewrite mode. In Rewrite mode write always succesfull
   * @param[in] value - value by constant reference to write
   * @return bool - result of writing
   */
  bool write(const value_type &value) noexcept {
    if (_isFull)
      return false;

    if (!_isEmpty && (_end() == _begin())) {
      ++_begin;
      _isOverwritten = true;
    }

    _data[_end++] = value;
    _isEmpty = false;

    _isFull = (_reWriteMode == rwType::nonRwBuf) && (_end() == _begin());

    return true;
  }

  /**
   * @brief Extracting the first item from the buffer
   * @note Returns the result of extracting @n
   * Unsuccessful extracting only when the buffer is empty.
   * @param[out] value - reference for return value
   * @return bool - result of extracting
   */
  bool read(value_type &value) noexcept {
    if (_isEmpty)
      return false;
    value = _data[_begin++];
    if (_end() == _begin())
      _isEmpty = true;
    _isFull = false;

    return true;
  }

  /**
   * @brief Returns the number of items in the buffer
   */
  size_type count() noexcept {
    if (_isEmpty)
      return 0;
    if (_begin() >= _end())
      return SIZE - _begin() + _end();
    else
      return _end() - _begin();
  }

  /**
   * @brief Index of the buffer array with the first item
   */
  size_type begin() noexcept {
    return _begin();
  }

  /**
   * @brief Index of the buffer array after the last item
   */
  size_type end() noexcept {
    return _end();
  }

  /**
   * @brief The input operator as a replacement for the write() method
   * @note The result of the write is not checked here. @n
   * In Rewrite mode write always succesfull
   * @param[in] value - takes a value by constant reference
   * @return RwCBuf& - pointer to class instance
   */
  RwCBuf& operator<<(const value_type &value) noexcept {
    write(value);
    return *this;
  }

  /**
   * @brief The output operator as a replacement for the read() method
   * @note The result of the read is not checked here.
   * @param[out] value - return a value by reference
   * @return RwCBuf& - pointer to class instance
   */
  RwCBuf& operator>>(value_type &value) noexcept {
    read(value);
    return *this;
  }

  /**
   * @brief Direct access to the buffer array
   * @note Safe: out of bounds is not possible.
   * @param[in] i - index from 0 to SIZE-1
   * @return value_type& - reference to array item with index i
   */
  value_type& operator[](const size_type i) noexcept {
    return _data[(_begin() + i) % SIZE];
  }
};
