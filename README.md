# wakuwa9_Engine わくわくエンジン
![188_20231201013901](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/7bb21bac-1fde-4fd3-9ce5-5b568e9ee145)

 - 個人作品『NeaR:Replica』を制作するための自作エンジンです。
 - 作品のアプリケーション開発を円滑に進めるための機能や、ゲームのクオリティを上げるための機能を随時拡張しながら制作しています。 

# 作品紹介
## 作品名 NeaR:Replica
![スクリーンショット 2023-11-29 212334](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/4f758de6-e365-42f2-a764-3fce4539b8c8)

- XZ軸に移動、探索して敵を倒しながらボスを倒すのが目標の3Dアクションシューティングです。
- 無彩色の世界観をコンセプトに統一感に特にこだわって制作しています。

![スクリーンショット 2023-12-01 015006](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/f644c19c-caf2-4939-909d-39f6f2d37cee)
![スクリーンショット 2023-12-01 015024](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/d57530cf-3fde-45f3-b811-5251a6c0096b)

# ゲーム内で使っているエンジンの機能

## ポストエフェクトを使ったグリッチエフェクト

- ポストエフェクトを使用し、画面上にノイズが走るようなエフェクトを入れることができます。
## before
![スクリーンショット 2023-11-29 212334](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/4f758de6-e365-42f2-a764-3fce4539b8c8)

## after
![スクリーンショット 2023-12-01 015459](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/e245821d-69c2-4bea-bac7-3ee9da51f926)

## blender上で生成したオブジェクト配置を反映
- このゲームでは沢山のオブジェクトを配置するのでフィールド作成を円滑に進めるための機能です。
- blenderからオブジェクトの配置情報をPythonを使用した自作アドオン機能で.jsonファイルを出力し、ゲーム側で読み込み、配置することが可能です。
![スクリーンショット 2023-12-01 020411](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/610d7e7e-8684-4910-8264-64ed12684e84)
![スクリーンショット 2023-12-01 020451](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/0b4fbdf8-bc4a-474f-8c14-3cd84280dcbe)
- また、フィールドオブジェクトだけでなく、プレイヤーを配置した場所にスポーンさせるデバックポイントや、ボスなどのイベントシーンを発生させるオブジェクトなどにも対応しています。
- デバック中はImGuiを利用したリロードボタンを押すことで実行中でも配置情報を更新することができます。
![スクリーンショット 2023-12-01 020631](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/4dc40a44-3e12-4d97-8656-4c9dd1523bbe)

## フォグ機能
- フィールドの奥行きを表現するためのフォグを適応させることができます。

## before
![スクリーンショット 2023-12-01 021015](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/0db86907-74e2-40e0-9ea4-4b0f9fe5798d)


## after
![スクリーンショット 2023-12-01 021056](https://github.com/daiki-wakui/wakuwa9_Engine/assets/94943607/1a0d176a-6b08-48d0-a725-93085a6bb218)
