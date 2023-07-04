//
//  IlluminantIOSTests.swift
//  IlluminantIOSTests
//
//  Created by Joe Manto on 8/16/20.
//  Copyright © 2020 Joe Manto. All rights reserved.
//
let joe = 5 // hello
let bob = 5//hello
let job = 6  //test
let tet = 6// test

import XCTest
@testable import IlluminantIOS

class IlluminantIOSTests: XCTestCase {

    override func setUpWithError() throws {/* //hello*5 */
        // Put setup code here. This method is called before the invocation of each test method in the class.
        x = 5 // test
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
    }

    func testExample() throws {
        let x = 5/* this is a comment **/
        let y = 6 // this is a comment
        let j/* jd */ = 5
        let x/*jd*/=6
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
    }

    func testPerformanceExample() throws {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }
}
