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
 *  - File: perfect-cpp-source.cpp
 *  - Description: Perfect C++ source implementation template
 *  - Style: Google C++ Style with CMX preferences
 *  - Version: 1.0.0
 */

// ============================================================================
// Includes
// ============================================================================
// Include corresponding header first
#include "perfect-cpp-header.h"

// Then system headers
#include <algorithm>
#include <chrono>
#include <fstream>
#include <system_error>

// Project headers
#include "utils/error_handling.h"
#include "utils/performance.h"

// ============================================================================
// Anonymous Namespace for Internal Helpers
// ============================================================================
namespace {

// Internal constants
constexpr std::size_t kInitialBackoffMs = 100;
constexpr std::size_t kMaxBackoffMs = 5000;

// Internal helper functions
[[nodiscard]] auto calculate_backoff(std::size_t attempt) -> std::size_t {
    const auto backoff = kInitialBackoffMs * (1 << attempt);
    return std::min(backoff, kMaxBackoffMs);
}

auto log_operation(const std::string& operation,
                   const cmx::perfect::PerformanceMetrics& metrics) -> void {
    SPDLOG_INFO("{} completed: {}", operation, metrics.to_string());
}

} // anonymous namespace

// ============================================================================
// CMX Namespace Implementation
// ============================================================================
namespace cmx {
namespace perfect {

// ============================================================================
// PerfectExample Implementation
// ============================================================================

PerfectExample::PerfectExample(std::string name, std::size_t buffer_size)
    : name_(std::move(name))
    , endpoint_{"localhost", 8080} {
    
    // Validate input parameters
    if (name_.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
    
    if (buffer_size == 0) {
        throw std::invalid_argument("Buffer size must be positive");
    }
    
    // Initialize resources
    initialize_buffer(buffer_size);
    
    SPDLOG_DEBUG("PerfectExample '{}' created with {} byte buffer",
                name_, buffer_size);
}

auto PerfectExample::process_data(std::string_view input)
    -> std::pair<OperationResult, PerformanceMetrics> {
    
    // Start timing
    const auto start_time = std::chrono::steady_clock::now();
    
    // Validate input
    if (!validate_input(input)) {
        SPDLOG_WARN("Invalid input for processing: size={}", input.size());
        return {OperationResult::kInvalidInput, {}};
    }
    
    try {
        // Update request count
        request_count_.fetch_add(1, std::memory_order_relaxed);
        
        // Process data (example implementation)
        const auto bytes_processed = input.size();
        
        // Simulate processing
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        // Update buffer (example)
        if (buffer_.size() < bytes_processed) {
            buffer_.resize(bytes_processed);
        }
        std::memcpy(buffer_.data(), input.data(), bytes_processed);
        
        // Calculate metrics
        const auto end_time = std::chrono::steady_clock::now();
        PerformanceMetrics metrics{
            .bytes_processed = bytes_processed,
            .processing_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                end_time - start_time),
            .memory_usage = buffer_.capacity()
        };
        
        // Log completion
        log_operation("process_data", metrics);
        
        return {OperationResult::kSuccess, metrics};
        
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to process data: {}", e.what());
        return {OperationResult::kFailure, {}};
    }
}

auto PerfectExample::configure(const NetworkEndpoint& endpoint) -> bool {
    // Validate endpoint
    if (!validate_endpoint(endpoint)) {
        SPDLOG_ERROR("Invalid endpoint configuration: {}:{}",
                    endpoint.address, endpoint.port);
        return false;
    }
    
    // Update configuration
    endpoint_ = endpoint;
    
    SPDLOG_INFO("Configuration updated: {}:{}",
               endpoint_.address, endpoint_.port);
    
    return true;
}

// ============================================================================
// Private Methods Implementation
// ============================================================================

auto PerfectExample::initialize_buffer(std::size_t size) -> void {
    // Reserve capacity for performance
    buffer_.reserve(size);
    
    // Initialize with zeros
    buffer_.resize(size);
    std::fill(buffer_.begin(), buffer_.end(), std::byte{0});
    
    SPDLOG_DEBUG("Buffer initialized with {} bytes", size);
}

auto PerfectExample::validate_input(std::string_view input) const -> bool {
    // Check size limits
    if (input.empty()) {
        SPDLOG_WARN("Input is empty");
        return false;
    }
    
    if (input.size() > kMaxInputSize) {
        SPDLOG_WARN("Input too large: {} > {}", input.size(), kMaxInputSize);
        return false;
    }
    
    // Additional validation could go here
    // e.g., check for valid UTF-8, specific patterns, etc.
    
    return true;
}

auto PerfectExample::update_metrics(PerformanceMetrics& metrics) const -> void {
    // Update metrics with current state
    metrics.memory_usage = buffer_.capacity();
    
    // Could add more metrics here
    // e.g., cache hit rate, CPU usage, etc.
}

// ============================================================================
// Static Methods Implementation
// ============================================================================

auto PerfectExample::create_default() -> std::unique_ptr<PerfectExample> {
    try {
        // Use make_unique for exception safety
        auto instance = std::make_unique<PerfectExample>("default-instance");
        
        // Configure with default endpoint
        const NetworkEndpoint default_endpoint{"127.0.0.1", 8080};
        if (!instance->configure(default_endpoint)) {
            SPDLOG_ERROR("Failed to configure default instance");
            return nullptr;
        }
        
        return instance;
        
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to create default instance: {}", e.what());
        return nullptr;
    }
}

auto PerfectExample::validate_endpoint(const NetworkEndpoint& endpoint) -> bool {
    // Validate address
    if (endpoint.address.empty()) {
        SPDLOG_WARN("Endpoint address is empty");
        return false;
    }
    
    // Validate port range
    if (endpoint.port == 0 || endpoint.port > 65535) {
        SPDLOG_WARN("Invalid port number: {}", endpoint.port);
        return false;
    }
    
    // Additional validation could include:
    // - DNS resolution
    // - Network connectivity test
    // - Security checks
    
    return true;
}

// ============================================================================
// Free Functions Implementation
// ============================================================================

auto open_file_safely(const std::string& filename)
    -> std::optional<UniqueFile> {
    
    // Open file with error checking
    FILE* file = std::fopen(filename.c_str(), "rb");
    if (!file) {
        const auto error_code = std::make_error_code(
            static_cast<std::errc>(errno));
        SPDLOG_ERROR("Failed to open file '{}': {}", filename, error_code.message());
        return std::nullopt;
    }
    
    // Create unique_ptr with custom deleter
    return UniqueFile{file, &std::fclose};
}

// ============================================================================
// Template Specializations
// ============================================================================
// If needed, provide template specializations here

} // namespace perfect
} // namespace cmx

// ============================================================================
// Unit Test Example (could be in separate file)
// ============================================================================
#ifdef UNIT_TESTS

#include <gtest/gtest.h>

namespace cmx::perfect::test {

class PerfectExampleTest : public ::testing::Test {
protected:
    void SetUp() override {
        instance_ = std::make_unique<PerfectExample>("test-instance");
    }
    
    void TearDown() override {
        instance_.reset();
    }
    
    std::unique_ptr<PerfectExample> instance_;
};

TEST_F(PerfectExampleTest, ConstructorValidatesInput) {
    EXPECT_THROW(PerfectExample(""), std::invalid_argument);
    EXPECT_THROW(PerfectExample("test", 0), std::invalid_argument);
}

TEST_F(PerfectExampleTest, ProcessDataValidatesInput) {
    const auto [result, metrics] = instance_->process_data("");
    EXPECT_EQ(result, OperationResult::kInvalidInput);
}

TEST_F(PerfectExampleTest, ConfigureValidatesEndpoint) {
    const NetworkEndpoint invalid_endpoint{"", 0};
    EXPECT_FALSE(instance_->configure(invalid_endpoint));
}

TEST_F(PerfectExampleTest, SuccessfulProcessing) {
    const std::string test_data = "Hello, World!";
    const auto [result, metrics] = instance_->process_data(test_data);
    
    EXPECT_EQ(result, OperationResult::kSuccess);
    EXPECT_GT(metrics.bytes_processed, 0);
    EXPECT_GT(metrics.processing_time.count(), 0);
}

} // namespace cmx::perfect::test

#endif // UNIT_TESTS