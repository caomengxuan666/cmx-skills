/*
 *  Copyright © 2026 [Your Name]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  - File: perfect-cpp-header.h
 *  - Description: Perfect C++ header template based on CMX coding style
 *  - Style: Google C++ Style with CMX preferences
 *  - Version: 1.0.0
 */

#pragma once

// ============================================================================
// Standard Library Headers
// ============================================================================
// Group includes logically and keep them minimal
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// ============================================================================
// Third-party Headers
// ============================================================================
// Only include what's absolutely necessary
#include <fmt/format.h>
#include <spdlog/spdlog.h>

// ============================================================================
// Project Headers
// ============================================================================
// Use forward declarations when possible to reduce dependencies
#include "base/common.h"
#include "utils/type_traits.h"

// ============================================================================
// Forward Declarations
// ============================================================================
// Reduce compilation dependencies
namespace other_module {
class SomeClass;
struct SomeData;
} // namespace other_module

// ============================================================================
// CMX Namespace Convention
// ============================================================================
namespace cmx {
namespace perfect {

// ============================================================================
// Constants and Macros
// ============================================================================
// Use constexpr for compile-time constants
constexpr std::size_t kDefaultBufferSize = 4096;
constexpr std::size_t kMaxConnections = 1024;

// Type-safe macros with clear naming
#define CMX_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            SPDLOG_ERROR("Assertion failed: {}", #expr); \
            std::abort(); \
        } \
    } while (0)

// ============================================================================
// Type Aliases
// ============================================================================
// Consistent naming: PascalCase for types, snake_case for variables
using ByteBuffer = std::vector<std::byte>;
using StringView = std::string_view;
using UniqueFile = std::unique_ptr<FILE, decltype(&std::fclose)>;

// ============================================================================
// Concepts (C++20)
// ============================================================================
// Use concepts for template constraints
template<typename T>
concept Serializable = requires(T t, std::ostream& os) {
    { t.serialize(os) } -> std::same_as<void>;
    { T::deserialize(os) } -> std::same_as<T>;
};

template<typename T>
concept Container = requires(T t) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    { t.size() } -> std::same_as<std::size_t>;
};

// ============================================================================
// Enumerations
// ============================================================================
// Use enum class for type safety
enum class LogLevel : std::uint8_t {
    kTrace,
    kDebug,
    kInfo,
    kWarning,
    kError,
    kCritical
};

enum class OperationResult {
    kSuccess,
    kFailure,
    kTimeout,
    kInvalidInput
};

// ============================================================================
// Structures
// ============================================================================
// Simple data aggregates
struct NetworkEndpoint {
    std::string address;
    std::uint16_t port;
    
    // Provide comparison for containers
    auto operator<=>(const NetworkEndpoint&) const = default;
};

struct PerformanceMetrics {
    std::size_t bytes_processed{0};
    std::chrono::milliseconds processing_time{0};
    std::size_t memory_usage{0};
    
    // Provide string representation
    [[nodiscard]] auto to_string() const -> std::string {
        return fmt::format("Processed {} bytes in {} ms ({} MB)",
                          bytes_processed,
                          processing_time.count(),
                          memory_usage / (1024 * 1024));
    }
};

// ============================================================================
// Classes
// ============================================================================
/**
 * @brief Perfect example class demonstrating CMX coding style
 * 
 * This class showcases:
 * - Google C++ style with CMX preferences
 * - Modern C++ features (C++20)
 * - RAII and exception safety
 * - Performance considerations
 * - Comprehensive documentation
 */
class PerfectExample {
public:
    // ========================================================================
    // Constructors and Destructor
    // ========================================================================
    
    /**
     * @brief Explicit constructor to prevent implicit conversions
     * @param name Unique identifier for this instance
     * @param buffer_size Initial buffer size (default: 4KB)
     */
    explicit PerfectExample(std::string name,
                           std::size_t buffer_size = kDefaultBufferSize);
    
    // Rule of Five: define or delete all special member functions
    PerfectExample(const PerfectExample&) = delete;
    PerfectExample& operator=(const PerfectExample&) = delete;
    PerfectExample(PerfectExample&&) noexcept = default;
    PerfectExample& operator=(PerfectExample&&) noexcept = default;
    ~PerfectExample() = default;
    
    // ========================================================================
    // Public Interface
    // ========================================================================
    
    /**
     * @brief Process data with performance tracking
     * @param input Data to process
     * @return Processing result with metrics
     * @throws std::invalid_argument if input is empty
     */
    [[nodiscard]] auto process_data(std::string_view input) 
        -> std::pair<OperationResult, PerformanceMetrics>;
    
    /**
     * @brief Configure with new settings
     * @param settings Configuration parameters
     * @return true if configuration was successful
     */
    auto configure(const NetworkEndpoint& endpoint) -> bool;
    
    /**
     * @brief Get current status
     * @return Status information
     */
    [[nodiscard]] auto get_status() const -> std::string;
    
    // ========================================================================
    // Static Methods
    // ========================================================================
    
    /**
     * @brief Create instance with default configuration
     * @return New PerfectExample instance
     */
    [[nodiscard]] static auto create_default() -> std::unique_ptr<PerfectExample>;
    
    /**
     * @brief Validate configuration parameters
     * @param endpoint Network endpoint to validate
     * @return Validation result
     */
    [[nodiscard]] static auto validate_endpoint(const NetworkEndpoint& endpoint) 
        -> bool;
    
private:
    // ========================================================================
    // Private Implementation
    // ========================================================================
    
    // Member variables: snake_case with trailing underscore
    std::string name_;
    ByteBuffer buffer_;
    NetworkEndpoint endpoint_;
    std::atomic<std::size_t> request_count_{0};
    
    // Private methods for internal implementation
    auto initialize_buffer(std::size_t size) -> void;
    auto validate_input(std::string_view input) const -> bool;
    auto update_metrics(PerformanceMetrics& metrics) const -> void;
    
    // Private constants
    static constexpr std::size_t kMaxInputSize = 10 * 1024 * 1024; // 10MB
};

// ============================================================================
// Free Functions
// ============================================================================
/**
 * @brief Utility function demonstrating perfect implementation
 * @tparam Container Type satisfying Container concept
 * @param container Input container
 * @return Processed result
 */
template<Container T>
[[nodiscard]] auto process_container(const T& container) 
    -> std::vector<typename T::value_type> {
    
    // Pre-allocate result for performance
    std::vector<typename T::value_type> result;
    result.reserve(container.size());
    
    // Use range-based for loop
    for (const auto& item : container) {
        // Use perfect forwarding when appropriate
        result.push_back(std::forward<decltype(item)>(item));
    }
    
    return result;
}

/**
 * @brief Safe resource acquisition with RAII
 * @param filename File to open
 * @return Unique handle to file
 */
[[nodiscard]] auto open_file_safely(const std::string& filename)
    -> std::optional<UniqueFile>;

} // namespace perfect
} // namespace cmx

// ============================================================================
// Inline Implementations
// ============================================================================
// Only inline trivial functions in headers
namespace cmx {
namespace perfect {

inline auto PerfectExample::get_status() const -> std::string {
    return fmt::format("{}: {} requests, endpoint {}:{}",
                      name_,
                      request_count_.load(),
                      endpoint_.address,
                      endpoint_.port);
}

} // namespace perfect
} // namespace cmx