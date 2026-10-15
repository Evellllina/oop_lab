#pragma once
#include <cstddef>
#include <ostream>

class String {
public:
    static constexpr size_t npos = static_cast<size_t>(-1);
    String();                  
    String(const char* str);       
    String(const String& other);     
    String(String&& other) noexcept;  
    String& operator=(String other); 
    ~String();

    void swap(String& other) noexcept;

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }
    const char* c_str() const noexcept;

    char& operator[](size_t i);
    const char& operator[](size_t i) const;
    char& at(size_t i);        
    const char& at(size_t i) const;

    void push_back(char c);
    void append(const char* str);
    void append(const String& other);
    void clear() noexcept;           
    void reserve(size_t new_capacity);

    size_t find(const char* substr, size_t from = 0) const;
    size_t find(const String& substr, size_t from = 0) const;
    void print(std::ostream& os) const;

    bool operator==(const String& other) const noexcept;
    bool operator!=(const String& other) const noexcept { return !(*this == other); }

    String rle_compress() const;

    void rle_decompress();

private:
    void grow_for(size_t needed);    

    char* data_;
    size_t size_;
    size_t capacity_;
};

std::ostream& operator<<(std::ostream& os, const String& s);
