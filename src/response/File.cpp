/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:50:32 by alappas           #+#    #+#             */
/*   Updated: 2024/06/03 14:50:34 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/File.hpp"

File::File() : fd_(0) {}

File::File(std::string path) : fd_(0)
{
    set_path(path);
}

File::~File()
{
    closeFile();
}

File::File(const File &rhs)
    : fd_(rhs.fd_), mime_ext_(rhs.mime_ext_), file_name_(rhs.file_name_),
      file_name_full_(rhs.file_name_full_), matches_(rhs.matches_), path_(rhs.path_) {
}

File &File::operator=(const File &rhs) {
    if (this != &rhs) {
        fd_ = rhs.fd_;
        mime_ext_ = rhs.mime_ext_;
        file_name_ = rhs.file_name_;
        file_name_full_ = rhs.file_name_full_;
        matches_ = rhs.matches_;
        path_ = rhs.path_;
    }
    return *this;
}

void File::set_path(std::string path, bool negotiation)
{
    path_ = removeDupSlashes(path);

    (negotiation) ? parseExtNegotiation() : parseExt();
}

void File::parseExt()
{
    std::string file = path_.substr(path_.find_last_of("/") + 1);

    if (file.empty())
        return;

    file_name_full_ = file;

    size_t lastDotPos = file.find_last_of(".");
    if (lastDotPos != std::string::npos && lastDotPos != 0)
    {
        mime_ext_ = file.substr(lastDotPos);

        std::transform(mime_ext_.begin(), mime_ext_.end(), mime_ext_.begin(), tolower);

        if (mime_ext_.length() > 15)
            mime_ext_ = mime_ext_.substr(0, 15);
    }

    file_name_ = (lastDotPos != std::string::npos)
                     ? file.substr(0, lastDotPos)
                     : file;
}

void File::parseExtNegotiation()
{
    std::string file = path_.substr(path_.find_last_of("/") + 1);

    if (file.empty())
        return;

    file_name_full_ = file;

    size_t lastDotPos = file.find_last_of(".");

    if (lastDotPos != std::string::npos && lastDotPos != 0)
    {
        std::string fileExtension = file.substr(lastDotPos);
        std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), tolower);

        std::string mimeType = getMimeType(fileExtension);

        if (mimeType == "application/octet-stream")
        {
            while (lastDotPos != std::string::npos && lastDotPos != 0)
            {
                fileExtension = file.substr(lastDotPos);
                std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), tolower);

                mimeType = getMimeType("." + fileExtension);

                if (mimeType != "application/octet-stream")
                    break;

                lastDotPos = file.find_last_of(".", lastDotPos - 1);
            }
        }

        mime_ext_ = mimeType;
    }

    file_name_ = (lastDotPos != std::string::npos)
                     ? file.substr(0, lastDotPos)
                     : file;
}

std::string File::getMimeType(const std::string ext)
{
    static MimeTypes mime;
    return mime.getType(ext);
}

std::string File::getStatusCode(int code)
{
    static HttpStatusCodes status_codes;
    return status_codes.getStatusCode(code);
}

bool File::openFile(bool create)
{
    close(fd_);

    int flags = create ? (O_CREAT | O_RDWR | O_TRUNC) : O_RDONLY;
    fd_ = open(path_.c_str(), flags, 0755);

    if (fd_ < 0)
    {
        std::string errorStr = "open: ";
        errorStr += strerror(errno);
        std::cout << CURSIVE_GRAY << "Error: " << errorStr << RESET << std::endl;
        return false;
    }

    return true;
}

void File::closeFile()
{
    if (fd_ <= 0)
        return;

    close(fd_);
    fd_ = -1;
}

void File::appendFile(const std::string &body)
{
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    std::string dirPath = "www";
    std::string newPath = dirPath + "/" + path_;
    if (!exists(dirPath))
    {
        if (mkdir(dirPath.c_str(), 0755) != 0)
        {
            std::cerr << "Error creating directory: " << strerror(errno) << std::endl;
            return;
        }
    }
    fd_ = open(newPath.c_str(), flags, 0644);
    if (fd_ < 0)
    {
        std::cerr << "Error opening file for append: " << strerror(errno) << std::endl;
        return;
    }

    ssize_t bytes_written = write(fd_, body.c_str(), body.length());
    if (bytes_written <= 0)
    {
        std::cerr << "Error appending to file: " << strerror(errno) << std::endl;
    }

    closeFile();
}

void File::appendFile(const std::string &body,std::string x_filename)
{
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    std::string dirPath = "www/serverDB/";

    std::string newPath = dirPath + x_filename;

    if (!exists(dirPath))
    {
        if (mkdir(dirPath.c_str(), 0755) != 0)
        {
            std::cerr << "Error creating directory: " << strerror(errno) << std::endl;
            return;
        }
    }
    fd_ = open(newPath.c_str(), flags, 0644);
    if (fd_ < 0)
    {
        std::cerr << "Error opening file for append: " << strerror(errno) << std::endl;
        return;
    }

    ssize_t bytes_written = write(fd_, body.c_str(), body.length());
    if (bytes_written <= 0)
    {
        std::cerr << "Error appending to file: " << strerror(errno) << std::endl;
    }

    closeFile();
}

bool File::deleteFile()
{
    if (is_directory()) {
        if (rmdir(path_.c_str()) != 0) {
            std::cerr << "Error deleting directory: " << strerror(errno) << std::endl;
            return false;
        }
        std::cout << "Directory deleted: " << path_ << std::endl;
    } else {
        if (unlink(path_.c_str()) != 0) {
            std::cerr << "Error deleting file: " << strerror(errno) << std::endl;
            return false;
        }
        std::cout << "File deleted: " << path_ << std::endl;
    }
    return true;
}

bool File::checkFileExists(const std::string &filePath)
{
    struct stat statbuf;
    if (stat(filePath.c_str(), &statbuf) != 0)
    {
        std::cerr << "Error checking existence for " << filePath << ": " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool File::exists()
{
    return checkFileExists(path_);
}

bool File::exists(const std::string &path)
{
    return checkFileExists(path);
}

bool File::is_directory()
{
    struct stat statbuf;
    if (!checkFileExists(path_))
        return false;

    if (stat(path_.c_str(), &statbuf) != 0)
    {
        std::cerr << "Error getting file info for " << path_ << ": " << strerror(errno) << std::endl;
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

bool File::is_file()
{
    struct stat fileStat;
    if (stat(path_.c_str(), &fileStat) != 0)
    {
        std::cerr << "Error getting file info: " << strerror(errno) << std::endl;
        return false;
    }

    return S_ISREG(fileStat.st_mode);
}

std::string File::last_modified()
{
    struct stat fileStat;
    if (stat(path_.c_str(), &fileStat) != 0)
    {
        std::cerr << "File does not exist or stat failed" << std::endl;
        return "Unknown";
    }

    time_t modifiedTime = fileStat.st_mtime;

    return formatHttpDate(modifiedTime);
}

std::string File::listDir(std::string &target)
{
    std::string body;
    std::vector<directory_listing> listing = getDirListings(path_);

    body.append(genHtmlHeader("Index of " + target));
    body.append("<h1>Index of " + target + "</h1><hr><pre>");
    body.append("<div style=\"display: grid; grid-template-columns: 1fr 1fr 1fr; align-items: center; padding: 0px 20px; font-size: 1rem; font-weight:bold;\">");
    body.append("<span>Directory</span>");
    body.append("<span>Date</span>");
    body.append("<span>Size</span>");
    body.append("</div><hr>\r\n");

    for (size_t i = 0; i < listing.size(); ++i)
    {
        body.append(formatListing(listing[i], target));
    }

    body.append("</pre><hr>");
    body.append(genHtmlFooter());

    return body;
}

std::string File::formatListing(const directory_listing &listing, const std::string &basePath)
{
    std::string formatted;

    std::string link = removeDupSlashes(basePath + "/" + listing.name_);
    formatted.append("<div style=\"display: grid; grid-template-columns: 1fr 1fr 1fr; align-items: center; padding: 0px 20px;\">");
    formatted.append("<a style=\"font-weight:bold;\" href=\"" + link + "\">" + listing.name_ + "</a>");
    formatted.append("<span>" + listing.date_ + "</span>");
    formatted.append("<span>" + (listing.is_dir_ ? "-" : ftos(listing.size_)) + "</span>");
    formatted.append("</div>\r\n");

    return formatted;
}

bool sortAutoListing(const directory_listing &a, const directory_listing &b)
{
    return a.name_ < b.name_;
}

std::vector<directory_listing> File::getDirListings(const std::string &dirPath)
{
    std::vector<directory_listing> listings;

    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return listings;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string fileName = entry->d_name;

        directory_listing listing = createListing(fileName, dirPath);
        listings.push_back(listing);
    }

    closedir(dir);
    std::sort(listings.begin(), listings.end(), sortAutoListing);
    return listings;
}

directory_listing File::createListing(const std::string &fileName, const std::string &dirPath)
{
    directory_listing listing;
    listing.name_ = fileName;

    std::string filePath = dirPath + "/" + fileName;
    struct stat fileStat;
    if (lstat(filePath.c_str(), &fileStat) != 0)
    {
        std::cerr << "Error getting file info: " << strerror(errno) << std::endl;
        return listing;
    }

    listing.is_dir_ = S_ISDIR(fileStat.st_mode);
    listing.size_ = fileStat.st_size;

    struct tm *timeinfo = localtime(&fileStat.st_mtime);
    char dateBuf[20];
    strftime(dateBuf, sizeof(dateBuf), "%d-%b-%Y %H:%M", timeinfo);
    listing.date_ = std::string(dateBuf);

    return listing;
}

std::string File::genHtmlHeader(const std::string &title)
{
    std::string header;

    header.append("<!DOCTYPE html>\r\n");
    header.append("<html>\r\n");
    header.append("<head>\r\n");
    header.append("<meta charset=\"UTF-8\">\r\n");
    header.append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n");
    header.append("<title>" + title + "</title>\r\n");
    header.append("</head>\r\n");
    header.append("<body>\r\n");

    return header;
}

std::string File::genHtmlFooter()
{
    std::string footer;
    footer.append("</body>\r\n");
    footer.append("</html>\r\n");
    return footer;
}

void File::print_dir_entry(struct dirent *ent) const
{
    std::cout << "Name: " << ent->d_name << std::endl;
    std::cout << "Inode: " << ent->d_ino << std::endl;
    std::cout << "Type: ";
    if (ent->d_type == DT_REG)
        std::cout << "Regular file" << std::endl;
    else if (ent->d_type == DT_DIR)
        std::cout << "Directory" << std::endl;
    else
        std::cout << "Other" << std::endl;
    std::cout << std::endl;
}

void File::print_file_info(const std::string &file) const
{
    struct stat file_stat;
    const std::string &filename = removeDupSlashes(path_ + file);
    if (stat(filename.c_str(), &file_stat) == 0)
    {
        std::cout << "File: " << filename << std::endl;
        std::cout << "Size: " << file_stat.st_size << " bytes" << std::endl;
        std::cout << "Permissions: " << (file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)) << std::endl;
        std::cout << "Last modified: " << ctime(&file_stat.st_mtime);
        (S_ISDIR(file_stat.st_mode) ? std::cout << "Type: Directory" : std::cout << "Type: Regular file") << std::endl;
    }
    else
        std::cout << "Failed to get information for " << filename << std::endl;
}

std::string File::find_index(std::vector<std::string> &indexes)
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir(path_.c_str());
    if (dir)
    {
        while ((ent = readdir(dir)))
        {
            // print_dir_entry(ent);
            if (std::find(indexes.begin(), indexes.end(), ent->d_name) != indexes.end())
            {
                std::string ret = "/";
                ret += ent->d_name;
                // print_file_info(ret);
                closedir(dir);
                return ret;
            }
        }
        closedir(dir);
    }
    else
        std::cout << "opendir : " << strerror(errno) << " of " << path_ << std::endl;

    return "";
}

void File::findMatchingFiles()
{
    DIR *dir;
    struct dirent *ent;
    std::string path = path_.substr(0, path_.find_last_of("/"));

    if (!matches_.empty())
        matches_.clear();
    dir = opendir(path.c_str());
    if (dir)
    {
        while ((ent = readdir(dir)))
        {
            std::string name(ent->d_name);
            if (name == file_name_full_)
                matches_.push_back(name);
            else if (name.find(file_name_) != std::string::npos &&
                     name.find(mime_ext_) != std::string::npos)
                matches_.push_back(name);
        }
        closedir(dir);
    }
    else
    {
        std::cerr << "opendir : " << strerror(errno) << " of " << path_ << std::endl;
    }
}

std::vector<std::string> &File::getMatches()
{
    return matches_;
}

std::string File::getContent()
{
    std::ifstream fileStream(path_.c_str(), std::ios::binary);
    if (!fileStream.is_open())
    {
        std::cerr << "Failed to open file: " << path_ << std::endl;
        return "";
    }

    // Get the size of the file for proper buffer allocation
    fileStream.seekg(0, std::ios::end);
    std::streamsize fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);

  
    if (!fileStream.read(buffer.data(), fileSize))
    {
        std::cerr << "Error reading file into buffer: " << path_ << std::endl;
        return "";
    }
    std::string content(buffer.begin(), buffer.end());

    return content;
}

int &File::getFd()
{
    return fd_;
}

std::string &File::getMimeExt()
{
    return mime_ext_;
}

std::string &File::getFilePath()
{
    return path_;
}

bool File::createFile(const std::string &content) {
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    fd_ = open(path_.c_str(), flags, 0644);
    if (fd_ < 0) {
        std::cerr << "Error creating file: " << strerror(errno) << std::endl;
        return false;
    }
    ssize_t bytes_written = write(fd_, content.c_str(), content.length());
    if (bytes_written <= 0) {
        std::cerr << "Error writing to file: " << strerror(errno) << std::endl;
        closeFile();
        return false;
    }
    closeFile();
    return true;
}

bool File::updateFile(const std::string &content) {
    int flags = O_WRONLY | O_TRUNC;

    fd_ = open(path_.c_str(), flags, 0644);
    if (fd_ < 0) {
        std::cerr << "Error opening file for update: " << strerror(errno) << std::endl;
        return false;
    }
    ssize_t bytes_written = write(fd_, content.c_str(), content.length());
    if (bytes_written <= 0) {
        std::cerr << "Error writing to file: " << strerror(errno) << std::endl;
        closeFile();
        return false;
    }
    closeFile();
    return true;
}

