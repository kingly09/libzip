//
//  LBZipArchive.h
//  libzip
//
//  Created by kingly on 15/12/18.
//  Copyright © 2015年 kingly. All rights reserved.
//

#import <Foundation/Foundation.h>

/* tm_unz contain date/time info */
typedef struct tm_unz_s
{
    unsigned int tm_sec;            /* seconds after the minute - [0,59] */
    unsigned int tm_min;            /* minutes after the hour - [0,59] */
    unsigned int tm_hour;           /* hours since midnight - [0,23] */
    unsigned int tm_mday;           /* day of the month - [1,31] */
    unsigned int tm_mon;            /* months since January - [0,11] */
    unsigned int tm_year;           /* years - [1980..2044] */
} tm_unz;

typedef struct unz_file_info_s
{
    unsigned long version;              /* version made by                 2 bytes */
    unsigned long version_needed;       /* version needed to extract       2 bytes */
    unsigned long flag;                 /* general purpose bit flag        2 bytes */
    unsigned long compression_method;   /* compression method              2 bytes */
    unsigned long dosDate;              /* last mod file date in Dos fmt   4 bytes */
    unsigned long crc;                  /* crc-32                          4 bytes */
    unsigned long compressed_size;      /* compressed size                 4 bytes */
    unsigned long uncompressed_size;    /* uncompressed size               4 bytes */
    unsigned long size_filename;        /* filename length                 2 bytes */
    unsigned long size_file_extra;      /* extra field length              2 bytes */
    unsigned long size_file_comment;    /* file comment length             2 bytes */
    
    unsigned long disk_num_start;       /* disk number start               2 bytes */
    unsigned long internal_fa;          /* internal file attributes        2 bytes */
    unsigned long external_fa;          /* external file attributes        4 bytes */
    
    tm_unz tmu_date;
} unz_file_info;

/* unz_file_info contain information about a file in the zipfile */
typedef struct unz_file_info64_s
{
    unsigned long version;              /* version made by                 2 bytes */
    unsigned long version_needed;       /* version needed to extract       2 bytes */
    unsigned long flag;                 /* general purpose bit flag        2 bytes */
    unsigned long compression_method;   /* compression method              2 bytes */
    unsigned long dosDate;              /* last mod file date in Dos fmt   4 bytes */
    unsigned long crc;                  /* crc-32                          4 bytes */
    unsigned long long compressed_size;   /* compressed size                 8 bytes */
    unsigned long long uncompressed_size; /* uncompressed size               8 bytes */
    unsigned long size_filename;        /* filename length                 2 bytes */
    unsigned long size_file_extra;      /* extra field length              2 bytes */
    unsigned long size_file_comment;    /* file comment length             2 bytes */
    
    unsigned long disk_num_start;       /* disk number start               2 bytes */
    unsigned long internal_fa;          /* internal file attributes        2 bytes */
    unsigned long external_fa;          /* external file attributes        4 bytes */
    
    tm_unz tmu_date;
    unsigned long long disk_offset;
    unsigned long size_file_extra_internal;
} unz_file_info64;

typedef struct unz_global_info_s
{
    unsigned long number_entry;         /* total number of entries in
                                         the central dir on this disk */
    
    unsigned long number_disk_with_CD;  /* number the the disk with central dir, used for spanning ZIP*/
    
    
    unsigned long size_comment;         /* size of the global comment of the zipfile */
} unz_global_info;

typedef struct unz_global_info64
{
    unsigned long long number_entry;         /* total number of entries in
                                              the central dir on this disk */
    
    unsigned long number_disk_with_CD;  /* number the the disk with central dir, used for spanning ZIP*/
    
    unsigned long size_comment;         /* size of the global comment of the zipfile */
} unz_global_info64;


@protocol LBZipArchiveDelegate;

@interface LBZipArchive : NSObject
/**
 *  缓存目录
 */
+(NSString *) cachesPath:(NSString *)directory;

// Unzip
+ (BOOL)unzipFileAtPath:(NSString *)path toDestination:(NSString *)destination;
+ (BOOL)unzipFileAtPath:(NSString *)path toDestination:(NSString *)destination delegate:(id<LBZipArchiveDelegate>)delegate;

+ (BOOL)unzipFileAtPath:(NSString *)path toDestination:(NSString *)destination overwrite:(BOOL)overwrite password:(NSString *)password error:(NSError * *)error;
+ (BOOL)unzipFileAtPath:(NSString *)path toDestination:(NSString *)destination overwrite:(BOOL)overwrite password:(NSString *)password error:(NSError * *)error delegate:(id<LBZipArchiveDelegate>)delegate;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
        progressHandler:(void (^)(NSString *entry, unz_file_info zipInfo, long entryNumber, long total))progressHandler
      completionHandler:(void (^)(NSString *path, BOOL succeeded, NSError *error))completionHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
              overwrite:(BOOL)overwrite
               password:(NSString *)password
        progressHandler:(void (^)(NSString *entry, unz_file_info zipInfo, long entryNumber, long total))progressHandler
      completionHandler:(void (^)(NSString *path, BOOL succeeded, NSError *error))completionHandler;

// Zip

// without password
+ (BOOL)createZipFileAtPath:(NSString *)path withFilesAtPaths:(NSArray *)paths;

+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath;

+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath keepParentDirectory:(BOOL)keepParentDirector;

// with password, password could be nil
+ (BOOL)createZipFileAtPath:(NSString *)path withFilesAtPaths:(NSArray *)paths withPassword:(NSString *)password;

+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath withPassword:(NSString *)password;

+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath keepParentDirectory:(BOOL)keepParentDirectory withPassword:(NSString *)password;

- (instancetype)initWithPath:(NSString *)path;
@property (NS_NONATOMIC_IOSONLY, readonly, getter = isOpen) BOOL open;
- (BOOL)writeFile:(NSString *)path withPassword:(NSString *)password;
- (BOOL)writeFileAtPath:(NSString *)path withFileName:(NSString *)fileName withPassword:(NSString *)password;
- (BOOL)writeData:(NSData *)data filename:(NSString *)filename withPassword:(NSString *)password;
@property (NS_NONATOMIC_IOSONLY, readonly, getter = isClosed) BOOL close;

@end

@protocol LBZipArchiveDelegate <NSObject>

@optional

- (void)zipArchiveWillUnzipArchiveAtPath:(NSString *)path zipInfo:(unz_global_info)zipInfo;

- (void)zipArchiveDidUnzipArchiveAtPath:(NSString *)path zipInfo:(unz_global_info)zipInfo unzippedPath:(NSString *)unzippedPath;

- (BOOL)zipArchiveShouldUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath fileInfo:(unz_file_info)fileInfo;

- (void)zipArchiveWillUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath fileInfo:(unz_file_info)fileInfo;

- (void)zipArchiveDidUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath fileInfo:(unz_file_info)fileInfo;

- (void)zipArchiveDidUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath unzippedFilePath:(NSString *)unzippedFilePath;

- (void)zipArchiveProgressEvent:(unsigned long long)loaded total:(unsigned long long)total;

- (void)zipArchiveDidUnzipArchiveFile:(NSString *)zipFile entryPath:(NSString *)entryPath destPath:(NSString *)destPath;

@end