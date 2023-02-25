#include <sycl/sycl.hpp>

int main() {
  // カーネルコード内で使用する 4 つの int バッファーを作成
  sycl::buffer<sycl::cl_int, 1> Buffer(4);

  // SYCL* キューを作成
  sycl::queue Queue;

  // カーネルのインデックス空間サイズ
  sycl::range<1> NumOfWorkItems{Buffer.size()};

  // キューへコマンドグループ (ワーク) を送信
  Queue.submit([&](sycl::handler &cgh) {

    // デバイス上のバッファーへの書き込み専用アクセサーを作成
    auto Accessor = Buffer.get_access<sycl::access::mode::write>(cgh);

    // カーネルを実行
    cgh.parallel_for<class FillBuffer>(
        NumOfWorkItems, [=](sycl::id<1> WIid) {
          // インデックスでバッファーを埋めます
          Accessor[WIid] = (sycl::cl_int)WIid.get(0);
        });
  });

  // ホスト上のバッファーへの読み取り専用アクセサーを作成。
  // キューのワークが完了するのを待機する暗黙のバリア
  const auto HostAccessor = Buffer.get_access<sycl::access::mode::read>();

  // 結果をチェック
  bool MismatchFound = false;
  for (size_t I = 0; I < Buffer.size(); ++I) {
    if (HostAccessor[I] != I) {
      std::cout << "The result is incorrect for element: " << I
                << " , expected: " << I << " , got: " << HostAccessor[I]
                << std::endl;
      MismatchFound = true;
    }
  }

  if (!MismatchFound) {
    std::cout << "The results are correct!" << std::endl;
  }

  return MismatchFound;
}