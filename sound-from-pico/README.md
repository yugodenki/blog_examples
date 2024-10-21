# blog/sound-from-pico README (日本語版)
Raspberry Pi PicoのPWM機能を使って、GPIOからオーディオ出力をするサンプルです。
オーディオデータはmp3を数値の配列にあらかじめ変換したものを使用します。

オーディオ信号はGPIO 7から出力されます。

詳しくは[ブログ記事](https://yugodenki.vercel.app/ja/blog/sound-from-pico-piezoelectric-buzzers)をご覧ください。
 
## 動作手順
### 前提
- Pico SDKがインストールされている必要があります。
インストールは公式手順「[Getting started with Raspberry Pi Pico-series](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)」にしたがってください。

### ビルド手順
リポジトリをクローンしたあと、もしまだであれば`sound-from-pico`ディレクトリに移動してください：

```bash
cd sound-from-pico
```

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

`make`を実行します。このコマンドの実行が成功すると`build`フォルダ内に`sound_from_pico.uf2`というファイルができているはずです。

```bash
make
```

ビルドしたプログラムをRaspberry Pi Pico上で動作させるには、まずPicoのBOOTSELボタンを押しながらPicoをコンピューターに接続します。コンピューターはPicoを大容量ストレージデバイスとして認識するので、先ほどの`sound_from_pico.uf2`ファイルをPicoにコピーします。コピーが終了するとただちにPicoは再起動し、プログラムが動作し始めます。

# blog/sound-from-pico README (English)
This is an example code I used in my blog post to demonstrate outputting audio signal from a Raspberry Pi Pico using the built-in PWM feature.
Audio data is provided as an array of integers converted from an mp3 file.

Please check out [the blog post](https://yugodenki.vercel.app/en/blog/sound-from-pico-piezoelectric-buzzers) for more details.


## Building and running the program
### Prerequisite
- Pico SDK needs to be installed.
Please follow the official instruction [Getting started with Raspberry Pi Pico-series](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) for the installment.

### Build instructions
After cloning the repo, move to the `sound-from-pico` directory if not yet:

```bash
cd sound-from-pico
```

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

Run make. After running this command, you should see `sound_from_pico.uf2` built within the `build` folder.

```bash
make
```

In order to run this program on the Raspberry Pi Pico, connect your Pico to your computer while pressing the BOOTSEL button. Your computer should detect the Pico as a USB mass storage device, so simply copy over `sound_from_pico.uf2` to Pico. Pico should immediately reboot and start running the program.
