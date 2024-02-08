<template>
	<view class="controlPad-container">
		<!-- test btn area -->
		<view class="test-area">
			<view class="row">
				<button class="test" @touchstart="handleButtonDown('boardled.01')"
					@touchend="handleButtonUp('boardled.01')"
					@touchcancel="handleButtonUp('boardled.01')">test1</button>
				<button class="test" @touchstart="handleButtonDown('boardled.00')"
					@touchend="handleButtonUp('boardled.00')"
					@touchcancel="handleButtonUp('boardled.00')">test0</button>
					
				<!-- 速度档 -->
				<button class="test" @touchstart="handleButtonDown('gear.00')"
					@touchend="handleButtonUp('gear.00')"
					@touchcancel="handleButtonUp('gear.00')">Gear0</button>
				<button class="test" @touchstart="handleButtonDown('gear.01')"
					@touchend="handleButtonUp('gear.01')"
					@touchcancel="handleButtonUp('gear.01')">Gear1</button>
				<button class="test" @touchstart="handleButtonDown('gear.02')"
					@touchend="handleButtonUp('gear.02')"
					@touchcancel="handleButtonUp('gear.02')">Gear2</button>
			</view>
			<view class="row">
				<!-- 自定义速度 -->
				<view class="slider-label">精准变速</view>
				<slider class="slider" value="100" @change="speedSliderChange" min="0" max="255" show-value />
			</view>
		</view>
		<!-- direct -->
		<view class="direct-container">
			<!-- 前 -->
			<view class="circle-btn" @touchstart="handleButtonDown('direct.01')"
				@touchend="handleButtonUp('direct.01')"
				@touchcancel="handleButtonUp('direct.01')">▲</view>
			<!-- 左 -->
			<view class="circle-btn" @touchstart="handleButtonDown('direct.03')"
				@touchend="handleButtonUp('direct.03')"
				@touchcancel="handleButtonUp('direct.03')">◀</view>
			<!-- 右 -->
			<view class="circle-btn" @touchstart="handleButtonDown('direct.04')"
				@touchend="handleButtonUp('direct.04')"
				@touchcancel="handleButtonUp('direct.04')">▶</view>
			<!-- 下 -->
			<view class="circle-btn" @touchstart="handleButtonDown('direct.02')"
				@touchend="handleButtonUp('direct.02')"
				@touchcancel="handleButtonUp('direct.02')">▼</view>
		</view>
		<!-- 功能控制 -->
		<view class="function-area">
			<!-- 下降 -->
			<button class="test" @touchstart="handleButtonDown('func.01')"
				@touchend="handleButtonUp('func.01')"
				@touchcancel="handleButtonUp('func.01')">下降</button>
			<!-- 上升 -->
			<button class="test" @touchstart="handleButtonDown('func.02')"
				@touchend="handleButtonUp('func.02')"
				@touchcancel="handleButtonUp('func.02')">上升</button>
			<!-- 清洗 -->
			<button class="test" @touchstart="handleButtonDown('func.03')"
				@touchend="handleButtonUp('func.03')"
				@touchcancel="handleButtonUp('func.03')">清洗</button>
			<!-- 复位 -->
			<button class="test" @touchstart="handleButtonDown('func.04')"
				@touchend="handleButtonUp('func.04')"
				@touchcancel="handleButtonUp('func.04')">复位</button>
		</view>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				options: {
					deviceId: "",
					serviceId: "",
					characteristicId: ""
				},
				longPressTimer: null,
				speedSliderChangeTimer: null,
				speedSliderValue: 100,
				signleActiveBtnCode: ""
			}
		},
		onLoad(options) {
			this.options = options
		},
		onHide() {
			this.handleButtonUp();
		},
		onUnhandledRejection() {
			this.handleButtonUp();
		},
		methods: {
			async speedSliderChange(e) {
				this.speedSliderValue = e.target.value;
				await this.writeBLECharacteristicValue("gear.99" + this.speedSliderValue);
			},
			string2Hex(str) {
				let val = ""
				for (let i = 0; i < str.length; i++) {
					if (val == "")
						val = str.charCodeAt(i).toString(16)
					else
						val += str.charCodeAt(i).toString(16)
				}
				return val
			},
			hex2ArrayBuffer(hex_str) {
				let typedArray = new Uint8Array(hex_str.match(/[\da-f]{2}/gi).map(function(h) {
					return parseInt(h, 16)
				}))
				let buffer = typedArray.buffer
				return buffer
			},
			// 写入数据，16进制的
			writeBLECharacteristicValue(val) {
				return new Promise((resolve, reject) => {
					var hex = this.string2Hex(val); //要发送的信息
					var buffer = this.hex2ArrayBuffer(hex)
					console.log("buffer =>", buffer)

					uni.writeBLECharacteristicValue({
						// writeType: "writeNoResponse",
						writeType: "write",
						// 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
						deviceId: this.options.deviceId,
						// 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
						serviceId: this.options.serviceId,
						// 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
						characteristicId: this.options.characteristicId,
						// 这里的value是ArrayBuffer类型
						value: buffer,
						success(res) {
							console.log('写入成功 success =>', res)
							resolve(res)
						},
						fail(res) {
							console.log("写入失败 =>", res)
							reject(res)
						}
					})
				})
			},
			// 按钮按下
			handleButtonDown(cmdVal) {
				return new Promise((resolve, reject) => {
					if (this.signleActiveBtnCode) {
						return false;
					}
					this.signleActiveBtnCode = cmdVal;
					
					if (this.longPressTimer) {
						return false;
					}
					
					// 长按模拟
					this.longPressTimer = setInterval(async () => {
						console.log("btn interval =>", cmdVal)
						try {
							await this.writeBLECharacteristicValue(cmdVal);
						} catch (e) {} finally {
							this.signleActiveBtnCode = "";
							resolve()
						}
					}, 20);
				})
			},
			// 按钮抬起
			handleButtonUp(cmdVal) {
				console.log("btn up =>", cmdVal)
				clearInterval(this.longPressTimer)
				this.longPressTimer = null;
			}
		}
	}
</script>
<style lang="scss" scoped>
	.controlPad-container {
		width: 100vw;
		height: 100vh;
		overflow: auto;
		.test-area {
			.row {
				display: flex;
				justify-content: center;
				align-items: center;
			}
			.slider-label {
				width: 5em;
			}
			.slider {
				width: calc(100% - 5em);
			}
			.test {
				width: 150upx;
				height: 90upx;
				border: 1px solid #ccc;
				padding: 5px;
			}
		}
		
		.function-area {
			display: flex;
			justify-content: center;
			align-items: center;
			width: 100vw;
			padding: 10upx 0;
		}

		.direct-container {
			position: relative;
			width: 100%;
			height: auto;

			.circle-btn {
				position: absolute;
				width: 100upx;
				height: 100upx;
				border-radius: 100%;
				border: 1px solid #ccc;
				background-color: #eee;
				text-align: center;
				line-height: 100upx;
				user-select: none;
				margin-top: 100upx;
			}

			.circle-btn:nth-child(1) {
				top: 50upx;
				left: calc(50% - 50upx);
			}

			.circle-btn:nth-child(2) {
				top: 200upx;
				left: calc(150upx);
			}

			.circle-btn:nth-child(3) {
				top: 200upx;
				right: calc(150upx);
			}

			.circle-btn:nth-child(4) {
				top: 350upx;
				left: calc(50% - 50upx);
			}
		}
	}
</style>