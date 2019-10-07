#include <zephyr.h>
#include <device.h>
//#include <logging/log_ctrl.h>
#include <logging/log.h>
//LOG_MODULE_REGISTER(myapp);

#include <sys/printk.h>
#include <drivers/spi.h>



void do_stuff(struct device *spi, struct spi_config *spi_cfg) {
	//LOG_INF("doiing shit\n");
	printk("doing stuff...\n");
	u8_t tx1[] = { 0xaa, 0xbb, 0xcc };
	u8_t rx1[3];
	struct spi_buf tx_bufs[] = {
                {
                        .buf = tx1,
                },
	};
	struct spi_buf_set tx = {
                .buffers = tx_bufs,
        };
	struct spi_buf rx_bufs[] = {
                {
                        .buf = rx1,
                },
	};
	struct spi_buf_set rx = {
                .buffers = rx_bufs,
        };
	int err = spi_transceive(spi, spi_cfg, &tx, &rx);
	if (err) {
		printk("spi tx failed: %d\n", err);
	}

}

static int stm32_spi_send(struct device *spi,
			  const struct spi_config *spi_cfg,
			  const uint8_t *data, size_t len)
{
	const struct spi_buf_set tx = {
		.buffers = &(const struct spi_buf){
			.buf = (uint8_t *)data,
			.len = len,
		},
		.count = 1,
	};

	return spi_write(spi, spi_cfg, &tx);
}

#if 0
static void stm32_spi_setup_nss_pin(void)
{
	static const struct pin_config pin_config = {
		.pin_num = STM32_PIN_PA4,
		.mode = STM32_PINMUX_ALT_FUNC_5 | STM32_PUSHPULL_PULLUP,
	};

	stm32_setup_pins(&pin_config, 1);
}
#endif

void main(void)
{
	//log_init();
        printk("man, fuck this shit! %s\n", CONFIG_BOARD);
	//LOG_INF("this is karls log....");
	


        struct device *spi;
        struct spi_config spi_cfg = {
		.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
		.frequency = 256000U,
		};
        int err;

        printk("karl's SPI example application\n");

        spi = device_get_binding(DT_SPI_1_NAME);
        if (!spi) {
                printk("Could not find SPI1 driver\n");
                return;
        }

#if 0
struct spi_cs_control cs_ctrl = {
  .gpio_dev = device_get_binding(DT_ST_STM32_SPI_1_CS_GPIOS_CONTROLLER),
  .gpio_pin = DT_ST_STM32_SPI_1_CS_GPIOS_PIN,
  .delay = 0
};
        spi_cfg.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB;
        spi_cfg.frequency = 256000U;
//	spi_cfg.cs = &cs_ctrl;
#endif

	int i = 0;
	while(1) {
		//while (log_process(false));
		
		//do_stuff(spi, &spi_cfg);
		uint8_t bb[] = {0xaa, 0xbb, 0xcc, i++};
		int rc = stm32_spi_send(spi, &spi_cfg, bb, sizeof(bb));
		if (rc) {
			printk("Ssend failed: %d\n", rc);
		} else {
			printk("did stuff well?\n");
		}
		
		k_sleep(200);
	}
}

