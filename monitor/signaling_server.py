import asyncio
import websockets
import json

connected = set()


async def handler(websocket):
    connected.add(websocket)
    print(f"[+] 新连接，当前 {len(connected)} 个客户端")
    try:
        async for message in websocket:
            print(f"[转发] {str(message)[:100]}")
            for other in list(connected - {websocket}):
                try:
                    await other.send(message)
                except Exception as e:
                    print(f"[!] 转发失败，移除: {e}")
                    connected.discard(other)
    except websockets.exceptions.ConnectionClosed as e:
        print(f"[-] 连接关闭: {e}")
    finally:
        connected.discard(websocket)
        print(f"[=] 清理完成，当前 {len(connected)} 个客户端")


async def main():
    print("[信令服务器] 启动在 ws://0.0.0.0:8765")
    async with websockets.serve(handler, "0.0.0.0", 8765):
        await asyncio.Future()


asyncio.run(main())