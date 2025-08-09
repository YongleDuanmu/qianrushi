<template>
	<view class="container">
		<!-- 头部标题 -->
		<view class="header">
			<text class="title">多媒体智能门禁系统</text>
			<text class="subtitle">Multimedia Smart Access Control System</text>
		</view>
		
		<!-- 状态卡片区域 -->
		<view class="cards-container">
			<!-- 灯状态卡片 -->
			<view class="card">
				<view class="card-header">
					<text class="card-title">灯光状态</text>
					<view class="status-indicator" :class="{'active': led == 1}"></view>
				</view>
				<view class="card-content">
					<text class="status-text">{{ led == 1 ? '开启' : '关闭' }}</text>
				</view>
			</view>
			
			<!-- 门状态卡片 -->
			<view class="card">
				<view class="card-header">
					<text class="card-title">门状态</text>
					<view class="status-indicator" :class="{'active': door == 1}"></view>
				</view>
				<view class="card-content">
					<text class="status-text">{{ door == 1 ? '开门' : '关门' }}</text>
				</view>
			</view>
			
			<!-- 门禁卡信息卡片 -->
			<view class="card">
				<view class="card-header">
					<text class="card-title">门禁卡</text>
				</view>
				<view class="card-content">
					<view class="card-id-container" v-if="cardid">
						<text class="card-id-number">{{ formatCardId(cardid) }}</text>
					</view>
					<text class="card-id-empty" v-else>无卡片信息</text>
				</view>
			</view>
			
			<!-- 拍照时间戳卡片 -->
			<view class="card">
				<view class="card-header">
					<text class="card-title">拍照时间</text>
				</view>
				<view class="card-content">
					<text class="timestamp">{{ carmtime || '暂无记录' }}</text>
				</view>
			</view>
		</view>
		
		<!-- 控制区域 -->
		<view class="control-section">
			<view class="control-card">
				<view class="control-header">
					<text class="control-title">远程控制</text>
				</view>
				<view class="control-content">
					<view class="control-item">
						<text class="control-label">LED4 灯光控制</text>
						<switch :checked="led4 == 1" @change="control_led4" color="#007AFF" />
					</view>
					<view class="control-item">
						<text class="control-label">门锁控制</text>
						<switch :checked="door == 1" @change="control_door" color="#28a745" />
					</view>
				</view>
			</view>
		</view>
		
		<!-- 刷新按钮 -->
		<view class="refresh-section">
			<button class="refresh-btn" @click="fetchDevData">
				<text class="refresh-text">刷新数据</text>
			</button>
		</view>
	</view>
</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')

	export default {
		data() {
			return {
				cardid: '',
				led: 0,
				led4: 0,
				carmtime: '',
				door: 0,
				token: '',
				timer: null // 用于存储定时器引用
			}
		},
		onLoad() {
			const params = {
				author_key: 'EYxoRwKPv9tlGUFD9/szoA+vORbwmPj6azv5YJHvyGMtBgPNXXwaUZGOl/EoVxC2',
				version: '2022-05-01',
				user_id: '295961',
			}

			this.token = createCommonToken(params);
		},
		onShow() {
			this.fetchDevData();
			// 设置3秒自动刷新
			this.timer = setInterval(()=>{
				this.fetchDevData();
			}, 3000);
		},
		onHide() {
			// 页面隐藏时清除定时器，避免内存泄漏
			if (this.timer) {
				clearInterval(this.timer);
				this.timer = null;
			}
		},
		methods: {
			//获取onenet上的数据
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', 
					method:'GET',
					data: {
						product_id:'ghCtwz980W',
						device_name:'dmylgd32'
					},
					header: {
						'authorization': this.token 
					},
					success: (res) => {
						console.log(res.data);
						
						this.cardid = res.data.data[0].value;
						this.carmtime = res.data.data[1].value;
						this.led = res.data.data[3].value; // 数字值 1 或 0
						this.led4 = res.data.data[4].value;
						this.door = res.data.data[2].value; // 数字值 1 或 0
					}
				});
			},
			
			control_led4(event) {
			    console.log('LED4控制事件:', event);
			    let value = event.detail.value;
			    let ledValue = value ? 1 : 0;
			    
			    console.log('发送LED4控制命令:', { led4: ledValue });
			    
			    uni.request({
			        url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', 
			        method:'POST',
			        data: {
			            product_id:'ghCtwz980W',
			            device_name:'dmylgd32',
			            params:{ "led4": ledValue }
			        },
			        header: {
			            'authorization': this.token,
			            'Content-Type': 'application/json'
			        },
			        success: (res) => {
			            console.log('LED控制响应:', res);
			            if (res.statusCode === 200 && res.data && res.data.code === 200) {
			                console.log('LED控制成功');
			                uni.showToast({
			                    title: '控制成功',
			                    icon: 'success'
			                });
			                setTimeout(() => {
			                    this.fetchDevData();
			                }, 500);
			            } else {
			                console.log('控制失败:', res.data);
			                uni.showToast({
			                    title: '控制失败',
			                    icon: 'none'
			                });
			            }
			        },
			        fail: (err) => {
			            console.error('LED控制请求失败:', err);
			            uni.showToast({
			                title: '网络错误',
			                icon: 'none'
			            });
			        }
			    });
			},
			
			control_door(event) {
			    console.log('门控制事件:', event);
			    let value = event.detail.value;
			    let doorValue = value ? 1 : 0;
			    
			    console.log('发送门控制命令:', { door: doorValue });
			    
			    uni.request({
			        url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', 
			        method:'POST',
			        data: {
			            product_id:'ghCtwz980W',
			            device_name:'dmylgd32',
			            params:{ "door": doorValue }
			        },
			        header: {
			            'authorization': this.token,
			            'Content-Type': 'application/json'
			        },
			        success: (res) => {
			            console.log('门控制响应:', res);
			            if (res.statusCode === 200 && res.data && res.data.code === 200) {
			                console.log('门控制成功');
			                uni.showToast({
			                    title: '控制成功',
			                    icon: 'success'
			                });
			                setTimeout(() => {
			                    this.fetchDevData();
			                }, 500);
			            } else {
			                console.log('控制失败:', res.data);
			                uni.showToast({
			                    title: '控制失败',
			                    icon: 'none'
			                });
			            }
			        },
			        fail: (err) => {
			            console.error('门控制请求失败:', err);
			            uni.showToast({
			                title: '网络错误',
			                icon: 'none'
			            });
			        }
			    });
			},
			
			// 格式化门禁卡ID显示
			formatCardId(cardId) {
				if (!cardId) return '';
				// 每4位添加一个空格，提高可读性
				return cardId.replace(/(.{4})/g, '$1 ').trim();
			}
		}
	}
</script>

<style>
	.container {
		padding: 20rpx;
		background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
		min-height: 100vh;
	}

	.header {
		text-align: center;
		padding: 40rpx 0;
	}

	.title {
		font-size: 48rpx;
		font-weight: bold;
		color: #ffffff;
		display: block;
		margin-bottom: 10rpx;
		text-shadow: 0 2rpx 4rpx rgba(0,0,0,0.3);
	}

	.subtitle {
		font-size: 24rpx;
		color: rgba(255,255,255,0.9);
	}

	.cards-container {
		display: grid;
		grid-template-columns: 1fr 1fr;
		gap: 20rpx;
		margin-bottom: 30rpx;
	}

	.card {
		background: rgba(255, 255, 255, 0.95);
		border-radius: 20rpx;
		padding: 30rpx;
		box-shadow: 0 8rpx 16rpx rgba(0,0,0,0.1);
		backdrop-filter: blur(10rpx);
	}

	.card-header {
		display: flex;
		justify-content: space-between;
		align-items: center;
		margin-bottom: 20rpx;
	}

	.card-title {
		font-size: 28rpx;
		font-weight: bold;
		color: #333;
	}

	.status-indicator {
		width: 20rpx;
		height: 20rpx;
		border-radius: 50%;
		background-color: #ccc;
	}

	.status-indicator.active {
		background-color: #4cd964;
		box-shadow: 0 0 10rpx #4cd964;
	}

	.card-content {
		text-align: center;
	}

	.status-text {
		font-size: 32rpx;
		font-weight: bold;
		color: #007AFF;
	}

	.card-id-container {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
		min-height: 80rpx;
	}

	.card-id-number {
		font-family: 'Courier New', monospace;
		font-size: 36rpx;
		font-weight: bold;
		color: #2c3e50;
		letter-spacing: 2rpx;
		text-shadow: 0 2rpx 4rpx rgba(0,0,0,0.1);
		word-break: break-all;
		line-height: 1.4;
	}

	.card-id-empty {
		font-size: 26rpx;
		color: #999;
		font-style: italic;
	}

	.timestamp {
		font-size: 24rpx;
		color: #666;
	}

	.control-section {
		margin-bottom: 30rpx;
	}

	.control-card {
		background: rgba(255, 255, 255, 0.95);
		border-radius: 20rpx;
		padding: 30rpx;
		box-shadow: 0 8rpx 16rpx rgba(0,0,0,0.1);
		backdrop-filter: blur(10rpx);
	}

	.control-header {
		margin-bottom: 30rpx;
	}

	.control-title {
		font-size: 32rpx;
		font-weight: bold;
		color: #333;
	}

	.control-item {
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 20rpx 0;
		border-bottom: 1rpx solid #eee;
	}

	.control-item:last-child {
		border-bottom: none;
	}

	.control-label {
		font-size: 28rpx;
		color: #333;
	}

	.refresh-section {
		text-align: center;
	}

	.refresh-btn {
		background: linear-gradient(45deg, #007AFF, #00D1FF);
		border: none;
		border-radius: 50rpx;
		padding: 20rpx 60rpx;
		box-shadow: 0 8rpx 20rpx rgba(0,122,255,0.3);
	}

	.refresh-text {
		color: white;
		font-size: 32rpx;
		font-weight: bold;
	}
</style>