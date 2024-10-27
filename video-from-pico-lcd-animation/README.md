# blog/video-from-pico-lcd-animation README (日本語版)
Raspberry Pi PicoからLCDに映像を出力するサンプルです。
連続する画像データを表示することで、GIF画像のような映像を表示します。
画像データはあらかじめCヘッダファイルに変換したものを使用します。

このサンプルは[こちらのブログ記事](https://yugodenki.vercel.app/ja/blog/video-from-pico-lcd-optimization)用に作成したものです。
 
## 動作手順
### 前提
- Pico SDKがインストールされている必要があります。
インストールは公式手順「[Getting started with Raspberry Pi Pico-series](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)」にしたがってください。

### ビルド手順
リポジトリをクローンしたあと、もしまだであれば`video-from-pico-lcd-animation`ディレクトリに移動してください。

ビルドファイルを格納するフォルダを作成します：

```bash
mkdir build
```

ビルドフォルダに移動します：

```bash
cd build
```

CMakeを実行します：

```bash
cmake ..
```

`make`を実行します。このコマンドの実行が成功すると`build`フォルダ内に`.uf2`ファイルができているはずです。

```bash
make
```

ビルドしたプログラムをRaspberry Pi Pico上で動作させるには、まずPicoのBOOTSELボタンを押しながらPicoをコンピューターに接続します。コンピューターはPicoを大容量ストレージデバイスとして認識するので、先ほどの`.uf2`ファイルをPicoにコピーします。コピーが終了するとただちにPicoは再起動し、プログラムが動作し始めます。

# blog/video-from-pico-lcd-animation README (English)
This is an example code I used in my [blog post](https://yugodenki.vercel.app/en/blog/video-from-pico-lcd-optimization) to demonstrate outputting animation to an LCD module using the Raspberry Pi Pico.
It displays a sequence of images similar to a GIF.
The image data is provided as a C header file that is preprepared.

## Building and running the program
### Prerequisite
- Pico SDK needs to be installed.
Please follow the official instruction [Getting started with Raspberry Pi Pico-series](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) for the installment.

### Build instructions
After cloning the repo, move to the `video-from-pico-lcd-animation` directory if not yet.

Create a directory for the output build files:

```bash
mkdir build
```

Move to the `build` folder:

```bash
cd build
```

Run CMake:

```bash
cmake ..
```

Run make. After running this command, you should see a `.uf2` file built within the `build` folder.

```bash
make
```

In order to run this program on the Raspberry Pi Pico, connect your Pico to your computer while pressing the BOOTSEL button. Your computer should detect the Pico as a USB mass storage device, so simply copy over the `.uf2` file to Pico. Pico should immediately reboot and start running the program.
