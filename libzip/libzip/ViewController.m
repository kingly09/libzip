//
//  ViewController.m
//  libzip
//
//  Created by kingly on 15/12/15.
//  Copyright © 2015年 kingly. All rights reserved.
//

#import "ViewController.h"

#import "Objective-Zip.h"
#import "Objective-Zip+NSError.h"


#define HUGE_TEST_BLOCK_LENGTH             (50000LL)
#define HUGE_TEST_NUMBER_OF_BLOCKS        (100000LL)


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [self test01ZipAndUnzip];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) test01ZipAndUnzip {
    NSString *documentsDir= [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSString *filePath= [documentsDir stringByAppendingPathComponent:@"test.zip"];
    
    NSLog(@"filePath::%@",filePath);
    
        [[NSFileManager defaultManager] removeItemAtPath:filePath error:NULL];
        
        NSLog(@"Test 1: opening zip file for writing...");
        
        OZZipFile *zipFile= [[OZZipFile alloc] initWithFileName:filePath mode:OZZipFileModeCreate];
        //ZipFileModeUnzip是解压模式(读模式)，ZipFileModeCreate创建并写入压缩文件(写模式)，ZipFileModeAppend不用说就是追加模式喽。
        //向当前Zip文件中添加文件需要使用ZipFileModeAppend模式
        
        
        
        
        NSLog(@"Test 1: adding first file...");
        
        OZZipWriteStream *stream1= [zipFile writeFileInZipWithName:@"abc.txt"compressionLevel:OZZipCompressionLevelBest];
        
        //compressionLevel指示压缩率级别，可以选择ZipCompressionLevelFast(最快), ZipCompressionLevelBest(最大压缩率)，ZipCompressionLevelNone(不压缩)
        

        
        NSLog(@"Test 1: writing to first file's stream...");
        
        NSString *text= @"abc";
        [stream1 writeData:[text dataUsingEncoding:NSUTF8StringEncoding]];
        
        NSLog(@"Test 1: closing first file's stream...");
        
        [stream1 finishedWriting];
        
        NSLog(@"Test 1: adding second file...");
    
        NSString *file2name= @"x/y/z/xyz.txt";
        OZZipWriteStream *stream2= [zipFile writeFileInZipWithName:file2name compressionLevel:OZZipCompressionLevelNone];
        

        
        NSLog(@"Test 1: writing to second file's stream...");

        NSString *text2= @"XYZ";
        [stream2 writeData:[text2 dataUsingEncoding:NSUTF8StringEncoding]];
        
        NSLog(@"Test 1: closing second file's stream...");
        
        [stream2 finishedWriting];
        
        NSLog(@"Test 1: closing zip file...");
        
        [zipFile close];

        NSLog(@"Test 1: opening zip file for reading...");
        
        OZZipFile *unzipFile= [[OZZipFile alloc] initWithFileName:filePath mode:OZZipFileModeUnzip];
        

        
        NSLog(@"Test 1: reading file infos...");
        
        NSArray *infos= [unzipFile listFileInZipInfos];


        
        OZFileInZipInfo *info1= [infos objectAtIndex:0];
        
        
        OZFileInZipInfo *info2= [infos objectAtIndex:1];
      
        
        NSLog(@"Test 1: - %@ %@ %lu (%ld)", info2.name, info2.date, (unsigned long) info2.size, (long) info2.level);

        NSLog(@"Test 1: opening first file...");
        
        [unzipFile goToFirstFileInZip];
        OZZipReadStream *read1= [unzipFile readCurrentFileInZip];
        

        
        NSLog(@"Test 1: reading from first file's stream...");
        
        NSMutableData *data1= [[NSMutableData alloc] initWithLength:256];
        NSUInteger bytesRead1= [read1 readDataWithBuffer:data1];

        
        NSString *fileText1= [[NSString alloc] initWithBytes:[data1 bytes] length:bytesRead1 encoding:NSUTF8StringEncoding];


        NSLog(@"Test 1: closing first file's stream... 显示内容：：%@",fileText1);
        
        [read1 finishedReading];
    

        NSLog(@"Test 1: opening second file...");
        
        [unzipFile locateFileInZip:file2name];
        OZZipReadStream *read2= [unzipFile readCurrentFileInZip];
        


        NSLog(@"Test 1: reading from second file's stream...");

        NSMutableData *data2= [[NSMutableData alloc] initWithLength:256];
        NSUInteger bytesRead2= [read2 readDataWithBuffer:data2];
        
 
        
        NSString *fileText2= [[NSString alloc] initWithBytes:[data2 bytes] length:bytesRead2 encoding:NSUTF8StringEncoding];
  
        
        NSLog(@"Test 1: closing second file's stream... 显示内容：：%@",fileText2);
        
        [read2 finishedReading];

        NSLog(@"Test 1: closing zip file...");
        
        [unzipFile close];
        
        NSLog(@"Test 1: test terminated succesfully");
    
//    } @catch (OZZipException *ze) {
//        NSLog(@"Test 1: zip exception caught: %ld - %@", (long) ze.error, [ze reason]);
//        
//        
//    } @catch (NSException *e) {
//        NSLog(@"Test 1: generic exception caught: %@ - %@", [[e class] description], [e description]);
//        
//        
//    } @finally {
//        [[NSFileManager defaultManager] removeItemAtPath:filePath error:NULL];
//    }
}

@end
