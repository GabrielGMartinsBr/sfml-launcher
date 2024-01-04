#pragma once

#include <zlib.h>

#include <sstream>
#include <stdexcept>

#include "./AppDefs.h"

struct ZlibUtils {
  static app::String decompress(app::String &compressedContent)
  {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = 0;
    zs.next_in = Z_NULL;

    if (inflateInit(&zs) != Z_OK) {
      throw std::runtime_error("Failed to initialize zlib inflate.");
    }

    zs.avail_in = compressedContent.length();
    char *charArr = const_cast<char *>(compressedContent.data());
    zs.next_in = reinterpret_cast<Bytef *>(charArr);

    const int bufferSize = 1024;
    char buffer[bufferSize];

    app::String inflatedData;
    int status;

    do {
      zs.avail_out = bufferSize;
      zs.next_out = reinterpret_cast<Bytef *>(buffer);
      status = inflate(&zs, Z_NO_FLUSH);

      if (status != Z_OK && status != Z_STREAM_END) {
        inflateEnd(&zs);
        std::ostringstream errorMsg;
        errorMsg << "Error while inflating data: " << status;
        throw std::runtime_error(errorMsg.str());
      }

      inflatedData.append(buffer, bufferSize - zs.avail_out);

    } while (zs.avail_out == 0);

    inflateEnd(&zs);
    return inflatedData;
  }

  static app::String compress(const app::String &input)
  {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
      throw std::runtime_error("Failed to initialize zlib deflate.");
    }

    zs.next_in = reinterpret_cast<Byte *>(
      const_cast<char *>(input.data())
    );
    zs.avail_in = static_cast<uInt>(input.size());

    app::String deflatedData;
    int status;
    const int bufferSize = 1024;
    char buffer[bufferSize];

    do {
      zs.next_out = reinterpret_cast<Byte *>(buffer);
      zs.avail_out = bufferSize;

      status = deflate(&zs, Z_FINISH);

      if (deflatedData.size() < zs.total_out) {
        deflatedData.append(buffer, zs.total_out - deflatedData.size());
      }

    } while (status == Z_OK);

    deflateEnd(&zs);

    if (status != Z_STREAM_END) {
      std::ostringstream errMsg;
      errMsg << "Exception during zlib compression: (" << status << ") " << zs.msg;
      throw(std::runtime_error(errMsg.str()));
    }

    return deflatedData;
  }
};