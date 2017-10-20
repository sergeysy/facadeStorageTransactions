//#include <chrono>
#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>

#include "logger.hpp"
//#include "time.hpp"
#include "facadeStorageTransactions.hpp"

const auto COUNT_MSEC = 1000;

FacadeStorageTransaction::FacadeStorageTransaction(const std::string& directory) noexcept
    : directory_(directory)
{
}

FacadeStorageTransaction::~FacadeStorageTransaction()
{
}

void FacadeStorageTransaction::addTransaction(const std::pair<std::string, std::string>& transaction) noexcept
{
    try
    {
        std::cerr <<logger() << __FUNCTION__ << " add transaction." << std::endl;
        const boost::filesystem::path path(directory_);
        if(!boost::filesystem::exists(path))
        {
            std::cerr <<logger() << "Directory \"" << path.string() << "\" not exists. will create..." << std::endl;
            boost::filesystem::create_directories(path);
            std::cerr <<logger() << "Directory \"" << path.string() << "\" was created." << std::endl;
        }
        const auto pathFile = path/transaction.first;
        std::ofstream ofs (pathFile.c_str(), std::ofstream::out);

        if(!ofs.is_open())
        {
            std::cerr << logger() << "ERROR save transactions: \"" << pathFile << "\" . Transaction: \"" << transaction.second << "\"" << std::endl;
            return;
        }
        ofs << transaction.second;
        ofs.flush();
        ofs.close();
    }
    catch(const boost::filesystem::filesystem_error& ex)
    {
        std::cerr << logger() << "Filesystem error: " << ex.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << logger() << "Error. " << ex.what() << std::endl;
    }
}

std::vector<std::string> FacadeStorageTransaction::getTransactions() const noexcept
{
    std::vector<std::string> transactions;
    boost::filesystem::path targetDir(directory_);

    boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

    BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod))
    {
        if (is_regular_file(i))
        {
            std::ifstream ifs(i.string(), std::ifstream::in);
            if(!ifs.is_open())
            {
                std::cerr << logger() << "ERROR read transaction: \"" << i.string() << "\" ." << std::endl;
                continue;
            }
            std::string str;

            ifs.seekg(0, std::ios::end);
            str.reserve(ifs.tellg());
            ifs.seekg(0, std::ios::beg);

            str.assign((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

            transactions.push_back( str );
            ifs.close();
        }
    }
    return transactions;
}

void FacadeStorageTransaction::markTransactionAsSent() const noexcept
{
    boost::filesystem::path path_to_remove(directory_);
    for (boost::filesystem::directory_iterator end_dir_it, it(path_to_remove); it!=end_dir_it; ++it)
    {
        boost::filesystem::remove_all(it->path());
    }
}

size_t FacadeStorageTransaction::countNotUploadedTransactions() const noexcept
{
    return std::count_if(
            boost::filesystem::directory_iterator(boost::filesystem::path(directory_)),
            boost::filesystem::directory_iterator(),
            static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file) );

}
