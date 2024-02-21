#include "DisplayDriver.h"

DisplayDriver::DisplayDriver()
    : lgfx::LGFX_Device(),
      PeripheralAPI(ZLHS_EX_TO_STRING(DisplayDriver))
{
    this->setTCStackDepth(8192);
}

DisplayDriver::~DisplayDriver()
{
}

void DisplayDriver::screenInit()
{
    auto cfg1 = busInstance.config(); // バス設定用の構造体を取得します。

    // SPIバスの設定
    cfg1.spi_host = this->tftSPIDevice;
    cfg1.spi_mode = 0;          // SPI通信モードを設定 (0 ~ 3)
    cfg1.freq_write = 27000000; // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
    cfg1.freq_read = 27000000;  // 受信時のSPIクロック
    cfg1.spi_3wire = true;      // 受信をMOSIピンで行う場合はtrueを設定
    cfg1.use_lock = true;       // トランザクションロックを使用する場合はtrueを設定
    cfg1.dma_channel = 0;       // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
    cfg1.pin_sclk = ZLHS_TFT_SCK_PIN;
    cfg1.pin_mosi = ZLHS_TFT_MOSI_PIN;
    cfg1.pin_miso = ZLHS_TFT_MISO_PIN;
    cfg1.pin_dc = ZLHS_TFT_DC_PIN;

    busInstance.config(cfg1);           // 設定値をバスに反映します。
    panelInstance.setBus(&busInstance); // バスをパネルにセットします。

    auto cfg2 = panelInstance.config(); // 表示パネル設定用の構造体を取得します。

    cfg2.pin_cs = ZLHS_TFT_CS0_PIN;  // CSが接続されているピン番号   (-1 = disable)
    cfg2.pin_rst = ZLHS_TFT_RST_PIN; // RSTが接続されているピン番号  (-1 = disable)
    cfg2.pin_busy = -1;              // BUSYが接続されているピン番号 (-1 = disable)

    // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。
    cfg2.panel_width = 240;    // 実際に表示可能な幅
    cfg2.panel_height = 320;   // 実際に表示可能な高さ
    cfg2.offset_x = 0;         // パネルのX方向オフセット量
    cfg2.offset_y = 0;         // パネルのY方向オフセット量
    cfg2.offset_rotation = 0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
    cfg2.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
    cfg2.dummy_read_bits = 1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
    cfg2.readable = true;      // データ読出しが可能な場合 trueに設定
    cfg2.invert = false;       // パネルの明暗が反転してしまう場合 trueに設定
    cfg2.rgb_order = false;    // パネルの赤と青が入れ替わってしまう場合 trueに設定
    cfg2.dlen_16bit = false;   // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
    cfg2.bus_shared = false;   // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

    panelInstance.config(cfg2);
}

void DisplayDriver::touchInit()
{
    auto cfg = touchInstance.config();

    cfg.x_min = 0;
    cfg.x_max = 320;
    cfg.y_min = 0;
    cfg.y_max = 240;
    cfg.pin_int = ZLHS_TFT_TOUCH_INT_PIN;
    cfg.bus_shared = true;
    cfg.offset_rotation = 1;

    cfg.spi_host = this->tasSPIDevice;
    cfg.freq = 30000;
    cfg.pin_sclk = ZLHS_TFT_TOUCH_SCK_PIN;
    cfg.pin_mosi = ZLHS_TFT_TOUCH_MOSI_PIN;
    cfg.pin_miso = ZLHS_TFT_TOUCH_MISO_PIN;
    cfg.pin_cs = ZLHS_TFT_TOUCH_CS0_PIN;

    touchInstance.config(cfg);
    panelInstance.setTouch(&touchInstance);
}

void DisplayDriver::blInit()
{
    auto cfg = lightInstance.config(); // バックライト設定用の構造体を取得します。

    cfg.pin_bl = ZLHS_TFT_BL_PIN; // バックライトが接続されているピン番号
    cfg.invert = false;           // バックライトの輝度を反転させる場合 true
    cfg.freq = 44100;             // バックライトのPWM周波数
    cfg.pwm_channel = 7;          // 使用するPWMのチャンネル番号

    lightInstance.config(cfg);
    panelInstance.setLight(&lightInstance);
}

int DisplayDriver::load()
{
    this->screenInit();
    this->touchInit();
    this->blInit();

    csuper(lgfx::LGFX_Device)->setPanel(&panelInstance);

    this->setRotation(3);

    if (!this->begin())
    {
        return -1;
    }

    return 0;
}

int DisplayDriver::unload()
{
    return 0;
}

void DisplayDriver::touchXYMapUpdata(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax)
{
    auto cfg = touchInstance.config();

    cfg.x_min = xMin;
    cfg.x_max = xMax;
    cfg.y_min = yMin;
    cfg.y_max = yMax;

    touchInstance.config(cfg);
    panelInstance.setTouch(&touchInstance);
}
