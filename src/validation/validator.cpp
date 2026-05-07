#include "validator.h"
#include <stdexcept>
#include <string>
#include <regex>

namespace Validator
{
    // Validates a Resource
    ValidationResult validate(const Resource &resource)
    {
        ValidationResult result;

        // Fatal logic & overflow errors (Exceptions)
        if (resource.getTotalCopies() < 0)
        {
            throw std::underflow_error("CRITICAL: Total physical copies dropped below zero.");
        }

        if (resource.getAvailableCopies() < 0 || resource.getAvailableCopies() > resource.getTotalCopies())
        {
            throw std::logic_error("CRITICAL: Available copies exceed total inventory or are negative.");
        }

        if (resource.getPublicationYear() > 2100 || resource.getPublicationYear() < 1000)
        {
            throw std::out_of_range("CRITICAL: Publication year is mathematically out of acceptable bounds.");
        }

        // Standard data validation (Collect errors)
        if (resource.getTitle().empty())
        {
            result.isValid = false;
            result.errors.push_back("Error: Title cannot be empty.");
        }

        if (resource.getAuthor().empty())
        {
            result.isValid = false;
            result.errors.push_back("Error: Author cannot be empty.");
        }

        if (resource.getPublisher().empty())
        {
            result.isValid = false;
            result.errors.push_back("Error: Publisher cannot be empty.");
        }

        // ISBN must be 10 or 13 chars
        if (resource.getIsbn().empty() || (resource.getIsbn().length() != 10 && resource.getIsbn().length() != 13))
        {
            result.isValid = false;
            result.errors.push_back("Error: ISBN must be exactly 10 or 13 characters long.");
        }

        return result;
    }

    // Validates a User
    ValidationResult validate(const User &user)
    {
        ValidationResult result;

        // Fatal logic errors (Exceptions)
        if (user.getBalance() < 0.0)
        {
            throw std::logic_error("CRITICAL: User starting balance cannot mathematically be negative.");
        }

        if (user.getMembershipTypeId() <= 0)
        {
            throw std::invalid_argument("CRITICAL: Invalid Membership Type ID provided to constructor.");
        }

        // Standard data validation (Collect errors)
        if (user.getUsername().length() < 5)
        {
            result.isValid = false;
            result.errors.push_back("Error: Username must be at least 5 characters.");
        }

        if (user.getPassword().length() < 8)
        {
            result.isValid = false;
            result.errors.push_back("Error: Password must be at least 8 characters long.");
        }

        // Email format regex check
        const std::regex emailPattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
        if (!std::regex_match(user.getEmail(), emailPattern))
        {
            result.isValid = false;
            result.errors.push_back("Error: Invalid email format (must contain @ and . domain).");
        }

        return result;
    }
}