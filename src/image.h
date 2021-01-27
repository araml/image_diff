#pragma once

#include <string>
#include <cstddef>
#include <ostream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <error.h>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifdef __GNUC__
#pragma GCC system_header
#include <stb_image.h>
#pragma GCC system_header
#include <stb_image_write.h>
#endif


using uchar = unsigned char;

struct image_data {
    int width;
    int height;
    int channels;
    uchar *ptr;
};

 /*  So I can plug different image loaders
  */
static auto load_image(std::string_view path) -> image_data {
    image_data data;
    data.ptr = stbi_load(path.data(), &data.width,
                         &data.height, &data.channels, 0);
    /*
     *        int fd = open(path.data(), O_RDONLY);
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            LOG("Error getting file size");
        }
        uchar *file = reinterpret_cast<uchar *>(
            mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
        data = reinterpret_cast<T *>(
            stbi_load_from_memory(file, sb.st_size, &width, &height, &channels,
       0));
    */

    return data;
}

struct YIQ {
    float y;
    float i;
    float q;

    auto operator-(const YIQ rhs) -> YIQ {
        return { y - rhs.y, i - rhs.i, q - rhs.q };
    }
};

struct RGB {
    uchar r;
    uchar g;
    uchar b;

    auto operator==(const RGB &rhs) const -> bool {
        return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
    }

    auto operator!=(const RGB &rhs) const -> bool {
        return !(*this == rhs);
    }

    operator YIQ() {
        // No alpha blend
        float y = r * 0.29889531 + g * 0.58662247 + b * 0.11448223;
        float i = r * 0.59597799 - g * 0.27417610 - b * 0.32180189;
        float q = r * 0.21147017 - g * 0.52261711 + b * 0.31114694;
        return YIQ{y, i, q};
    }

    friend auto operator<<(std::ostream &os, const RGB &p) -> std::ostream&;
};

static auto blend(uchar color, uchar alpha) -> int  {
    return 255 + ((int)color - 255) * alpha;
}

struct RGBA {
    uchar r;
    uchar g;
    uchar b;
    uchar a;

    auto operator==(const RGBA &rhs) const -> bool {
        return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
    }

    auto operator!=(const RGBA &rhs) const -> bool {
        return !(*this == rhs);
    }

    operator YIQ() {
        float rr = r;
        float gg = g;
        float bb = b;
        if (a < 255) {
            rr = blend(r, a);
            gg = blend(g, a);
            bb = blend(b, a);
        }

        /*
        float rr = blend(r, a);
        float gg = blend(g, a);
        float bb = blend(b, a);
        */
        float y = rr * 0.29889531 + gg * 0.58662247 + bb * 0.11448223;
        float i = rr * 0.59597799 - gg * 0.27417610 - bb * 0.32180189;
        float q = rr * 0.21147017 - gg * 0.52261711 + bb * 0.31114694;

        //float y = rr * 0.29889531 + gg * 0.58662247 + bb * 0.11448223;
        //float i = rr * 0.59597799 - gg * 0.27417610 - bb * 0.32180189;
        //float q = rr * 0.21147017 - gg * 0.52261711 + bb * 0.31114694;
        return YIQ{y, i, q};
    }

    friend auto operator<<(std::ostream &os, const RGBA &p) -> std::ostream&;
};

auto operator<<(std::ostream &os, const RGB &p) -> std::ostream & {
    os << "(" << p.r << ", " << p.g << ", " << p.b << ")";
    return os;
}

auto operator<<(std::ostream &os, const RGBA &p) -> std::ostream & {
    os << "(" << p.r << ", " << p.g << ", " << p.b << ", " << p.a << ")";
    return os;
}


struct box {
    int h;
    int w;
};


template <typename T>
struct pixel_size {};

template <>
struct pixel_size<RGB> { constexpr static size_t value{3}; };

template <>
struct pixel_size<RGBA> { constexpr static size_t value{4}; };

#if __cplusplus > 201402L
template <typename T>
inline constexpr size_t pixel_size_v = pixel_size<T>::value;
#endif

template <typename T>
class row {
public:
    row(T *ptr, int width) : data(ptr), width(width) {}

    T &operator[](size_t k) {
        return data[k];
    }

private:
    T *data;
    int width;
};

struct image_size {
    int width;
    int height;

    auto operator==(const image_size &rhs) const -> bool {
        return width == rhs.width && height == rhs.height;
    }
};

template <typename T>
struct image {
    struct iterator_type {
        iterator_type(T *data) : data(data) { }

        auto operator++(int) -> iterator_type {
            data++;
            return *this;
        }

        auto operator++() -> iterator_type & {
            ++data;
            return *this;
        }

        auto operator*() -> T& {
            return *data;
        }

        auto operator==(const iterator_type& rhs) const -> bool {
            return data == rhs.data;
        }

        auto operator!=(const iterator_type& rhs) const -> bool {
            return !(*this == rhs);
        }

        T *data;
    };

    using iterator = iterator_type;
    using const_iterator = iterator_type;

    iterator begin() {
        return iterator(data);
    }

    iterator end() {
        return iterator(data + (width * height));
    }

    const_iterator cbegin() const {
        return iterator(data);
    }

    const_iterator cend() const {
        return iterator(data + (width * height));
    }

    image(T *data, size_t width, size_t height)
        : data(data), width(width), height(height) {
    }

    image(std::string_view path) {
        auto im_data = load_image(path);
        data = reinterpret_cast<T *>(im_data.ptr);
        width = im_data.width;
        height = im_data.height;
        channels = im_data.channels;

        assert(channels == pixel_size_v<T>);
    }

    image(image_size sz) : width(sz.width), height(sz.height) {
        data = new T[width * height];
        std::memset(data, 0, sizeof(T) * width * height);
    }

    image(image &&img)
        : width(img.width), height(img.height),
          channels(img.channels), data(std::move(img.data)) {
        img.data = nullptr;
    }

    // TOOD: no deallocation of memory now since the program finishes after processing
    // all the data
    // maybe think of doing dealloc if I want to share it as a lib or something
    ~image() {}

    row<T> operator[](size_t i) {
        return row<T>(&data[i * width], width);
    }

    auto size() const -> image_size {
        return {width, height};
    }

    auto save(std::string_view path) -> void {
        stbi_write_png_compression_level = 2;
        stbi_write_png(path.data(), width, height, pixel_size_v<T>, data,
                       width * pixel_size_v<T>);
    }

    auto operator==(const image<T> &rhs) const -> bool {
        if (size() != rhs.size())
            return false;

        for (auto it1 = cbegin(),
             it2 = rhs.cbegin(); it1 != cend() && it2 != rhs.cend();
             ++it1, ++it2) {
            if (*it1 != *it2)
                return false;
        }
        return true;
    }

    auto operator!=(const image<T> &rhs) const -> bool {
        return !(*this == rhs);
    }

    T *data;
    int width;
    int height;
    int channels;
};

struct image_info {
    int width;
    int height;
    int channels;
};

auto query_image_info(std::string_view path) -> image_info {
    image_info info;
    stbi_info(path.data(), &info.width, &info.height, &info.channels);
    return info;
}
