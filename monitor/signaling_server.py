import asyncio
import websockets
import json

client = set()

async def handler(websocket):
    client.add(websocket)
    print(f"[+]新连接，当前共{len(client)}个连接客户端")
    try:
        async for message in websocket:
            print(f"[转发]{message[:80]}...")
             # 把消息转发给其他所有客户端
            others = client - {websocket}
            if others:
                await asyncio.gather(*[c.send(message) for c in others])
    except websockets.exceptions.ConnectionClosedError:
        print(f"[-]连接关闭，当前共{len(client)}个连接客户端")

async def main():
    print("[信令服务器] 启动在 ws://0.0.0.0:8765")
    async with websockets.serve(handler, "0.0.0.0", 8765):
        await asyncio.Future()  # 永久运行

asyncio.run(main())

        