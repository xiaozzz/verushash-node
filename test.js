var crypto=require("./verushash.cc");
var sys=require("sys");
var fs=require("fs");
var test=require("mjsunit");


// Test hashing
var a0 = (new verushash.Hash).init("verus").update("Test123").digest("hex");
var a1 = (new verushash.Hash).init("verus").update("Test123").digest("binary");
var a3 = (new verushash.Hash).init("verus").update("Test123").digest(); // binary

// Test multiple updates to same hash
var h1 = (new verushash.Hash).init("verus").update("Test123").digest("hex");
var h2 = (new verushash.Hash).init("verus").update("Test").update("123").digest("hex");
test.assertEquals(h1, h2, "multipled updates");

