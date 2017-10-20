#pragma once

#include <string>

class FacadeStorageTransaction
{
public:
    explicit FacadeStorageTransaction(const std::string &directory) noexcept;
    virtual ~FacadeStorageTransaction();
    void addTransaction(const std::pair<std::string, std::string> &transaction) noexcept;
    std::vector<std::string> getTransactions() const noexcept;
    void markTransactionAsSent() const noexcept;
    size_t countNotUploadedTransactions() const noexcept;

private:
    std::string directory_;
};
