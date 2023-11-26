
#include <cassert>
#include <iostream>

#include "gtest/gtest.h"
#include "leveldb/db.h"

//
// git clone git@github.com:wuyuan1002/leveldb.git
//
// 下载 git submodule:
// cd third_party
// git submodule update --init
//
// 编写一个单测，用来测试 -- example/main.cpp
// 在CMakeLists.txt中增加一行（第391行）：leveldb_test("example/main.cpp")
//
// 开始编译:
// mkdir -p build && cd build
// cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
//
// 将include头文件和静态链接库文件libleveldb.a安装到/usr/local/include/ 和 /usr/local/lib下, 供其他项目直接使用,
// gcc编译的时候会默认查找/usr/include/、/usr/local/include/进行头文件include,
// 链接的时候会默认找/lib/、/usr/lib/、/usr/local/lib/下的静态链接库进行链接
// make install
//
// 执行单测:
// ./leveldb_tests
//
// 执行自己写的测试:
// ./main
//
// 参考链接:
// https://jasonkayzk.github.io/2022/12/09/LevelDB%E4%BD%BF%E7%94%A8%E7%A4%BA%E4%BE%8B/
// https://blog.csdn.net/Zhouzi_heng/article/details/126196074
// https://zhuanlan.zhihu.com/p/322520485

const std::string db_name = "/tmp/test_db";

leveldb::Options get_options() {
    leveldb::Options options;
    options.create_if_missing = true;
    return options;
}

leveldb::DB* init_db(leveldb::Options&& options) {
    leveldb::DB* db;
    leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
    assert(status.ok());
    return db;
}

TEST(LevelDBDemo, CRUD) {
    leveldb::DB* db = init_db(get_options());
    std::string k = "name";

    // Write data.
    leveldb::Status status = db->Put(leveldb::WriteOptions(), k, "test");
    ASSERT_TRUE(status.ok());

    // Read data.
    std::string val;
    status = db->Get(leveldb::ReadOptions(), k, &val);
    ASSERT_TRUE(status.ok());
    std::cout << "Get key: " << k << ", val: " << val << std::endl;
    ASSERT_EQ(val, "test");

    // Delete data.
    status = db->Delete(leveldb::WriteOptions(), "name");
    ASSERT_TRUE(status.ok());

    // Re-Get the key:
    status = db->Get(leveldb::ReadOptions(), k, &val);
    ASSERT_FALSE(status.ok());
    std::cout << "Get key after delete, status: " << status.ToString() << std::endl;
    ASSERT_TRUE(status.IsNotFound());

    delete db;
}

int main(int argc, char** argv) {
    printf("Running main() from %s\n", __FILE__);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}