//
//  JsonParserTests.m
//  JsonParserTests
//
//  Created by Ivan Cvetkov on 29.05.23.
//

#import <XCTest/XCTest.h>
#import </Users/ivancvetkov/Desktop/uni shit/uni/uni/main.cpp>

@interface JsonParserTests : XCTestCase

@end

@implementation JsonParserTests

string filePath = "/Users/ivancvetkov/Desktop/organisation1.json";
JsonInfo jsonInfo = JsonInfo::parseJsonFile(filePath);

- (void)testValidate {
    
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    XCTAssert(JsonInfo::validate(jsonInfo, filePath) == true);
}

- (void)testDelete {
    
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    XCTAssert(JsonInfo::deleteEntryByKey(jsonInfo, "management/directorId") == true);
}

- (void)testFind {
    
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    XCTAssert(JsonInfo::find(jsonInfo, "management") == true);
}
- (void)testSet {
    
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    XCTAssert(JsonInfo::set(jsonInfo, "management/directorId", "00000") == true);
}
- (void)testCreate {
    
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    XCTAssert(JsonInfo::create(jsonInfo, "default", "randomId", "1") == true);
}

@end
