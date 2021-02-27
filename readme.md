# 最初に

* [English](/readme-eng.md)

本readmeは大分古くなっていますので、追々更新します。

# インストールの方法

## Windows

Windows用バイナリをダウンロードし、適当なフォルダに展開します。後は展開したフォルダに移動し、hengbandをクリックすれば遊べます。

### Windows版独自の機能(by Kokaさん)

Option - 壁紙を使う で壁紙を設定できます。使用する壁紙は Option - 壁紙を選択 で選択できます。
  
## UNIX

ソースをダウンロードします。そして、

> tar -jxvf hengband-x.x.x.tar.bz2
> cd hengband-x.x.x
> ./configure
> make install

でコンパイル及びトップディレクトリへのコピーができます(x.x.x はバージョン番号)。
  
後は、ASCII文字画面なら、

> ./hengband -- -n＜ウィンドウの数＞  

タイルグラフィックス画面なら、

> ./hengband -g -- -n＜ウィンドウの数＞      ## 通常の8x8タイル
> ./hengband -g -- -b -n＜ウィンドウの数＞   ## 2倍幅サイズのタイル
> ./hengband -g -- -a -n＜ウィンドウの数＞   ## 16x16タイル(別配布のタイルが必要)

  等で遊べます。

  また、スーパーユーザーとして setgid を付けてインストールしたい場合は、

> ./configure --with-setgid=games
> make

  の後、rootとして

> make install  

を実行すれば、/usr/local/ 以下にインストールされます。インストール場所の変更等については ./configure --help を参照してください。

標準の表示フォントはメインウィンドウが

> "-*-*-medium-r-normal--24-*-*-*-*-*-iso8859-1,-*-*-medium-r-normal--24-*-*-*-*-*-jisx0208.1983-0"

で、サブウィンドウが

> "-*-*-medium-r-normal--16-*-*-*-*-*-iso8859-1,-*-*-medium-r-normal--16-*-*-*-*-*-jisx0208.1983-0"

ですが、環境変数 ANGBAND_X11_FONT_? や ANGBAND_X11_FONT で変更できます。
ANGBAND_X11_FONT_? は ?番目のウィンドウ(?は0から7)の設定になり、ANGBAND_X11_FONT は前者によって設定されなかったその他のウィンドウの設定になります。

また、ウィンドウの表示位置やサイズを変える事もできます。
?番目のウィンドウ(?は0から7)について、

* ANGBAND_X11_AT_X_?でウィンドウのX位置、
* ANGBAND_X11_AT_Y_?でウィンドウのY位置、
* ANGBAND_X11_COLS_?でウィンドウのXサイズ(単位は半角の文字数)、
* ANGBAND_X11_ROWS_?でウィンドウのYサイズ(単位は半角の文字数)

を設定できます。

XFree86 4.0 でXIMを使うときは、環境変数 XMODIFIERS に "@im=skkinput" や "@im=kinput2" と設定する必要があります。

## Macintosh

自分はMacintoshを所持していないので分かりません(^^;。http://www.boreas.dti.ne.jp/~xdd/index.html へどうぞ。


# 基本操作

  標準の設定ならば、テンキーで移動しフルキーでコマンド入力を行います。
  コマンドが分からない場合、リターンキー又は'x'キーを押すとコマンドメニューが
  出るので、そこから選んで実行することができます。
  コマンドの最後についているアルファベットはコマンドを実行するためのキーです。
  例えば、「休息する(R)」は'R'を押すことでも実行でき(大文字・小文字は区別)、
  「周りを調べる(l/x)」は標準のキー配置なら'l'、ローグライクキー配置なら'x'を
  押すことでも実行できます。

  最初は辺境の地のすぐ隣りにある「イークの洞穴」に行くことをお勧めします。
  ここは最深階に住んでいる「イークの大王」が強敵ですが、それ以外は安全な
  小さな洞穴です。このダンジョンを征覇できるくらい強くなったら、街の外へ出て
  いろいろなダンジョンへ潜ってみるといいでしょう。
  街の外へ出るときは、多くの食料を持っていかなければいけません。
  この世界は非常に広いので、街の外に出るときは'<'で広域マップに入り、
  目的地についたところで'>'で通常マップに戻る、という移動の仕方が便利です。
  荒野では道や草原は比較的安全ですが、溶岩地帯や海の上は強力なモンスターが
  いて危険です。こういったところへ行くためにはさらに強くならないといけません。

  辺境の地と同じ島にある火山は駆け出しの冒険者ではまだまだ歯が立ちません。
  もっと強くなってから戻ってくるといいでしょう。
  西に行くと辺境の地とモリバントの中間付近にオークの住む洞窟があり、
  その南の島にはミノタウロスが住むと噂される魔法の迷宮があります。
  他にも多くのダンジョンが勇敢な冒険者が訪れるのを待っています。
  このゲームのメインとなるダンジョンであり、最終目的である
  「混沌のサーペント」の棲む「鉄獄」のダンジョンはマップのほぼ中央、
  モリバントの北東に位置します。

## コマンド

  詳しいゲームの説明はオンライン・ヘルプ('?'キーを押して表示)を参照
  してください。ここでは基本的なコマンドだけ説明します。

  コマンドは大文字と小文字を区別します。
  多くのコマンドは小文字なのでCapsはOFFにしておきましょう。
  以下のコマンドが全てではありませんが、
  あまり使わないコマンドはリターンキーのメニューで選べば十分です。
  u/Z ってあるのは、オリジナルキー配置ならu、ローグライクキー配置ならZです。

* [移動]
   * テンキー(1-9) 各方向に移動(オリジナルキー配置)
   * h,j,k,l,y,u,b,n 各方向に移動(ローグライクキー配置)
   * 5/, その場にとどまる
   * < 上り階段を上る。地上では広域マップに入る。
   * \> 下り階段を下る。広域マップから通常のマップに戻る。
   * Shift+方向 走る
   * Ctrl+方向 モンスターがいれば攻撃、ドアがあれば開閉、岩があれば掘る
* [道具]
   * r 巻物を読む
   * q 薬を飲む
   * u/Z 杖を使う
   * a/z 魔法棒を使う
   * z/a ロッドを使う
   * E 食べ物を食べる
   * A 装備品の特殊能力を使う
   * F ランタンに油を補充する
   * f/t 飛び道具で攻撃する
* [行動]
   * d アイテムを落とす
   * g アイテムを拾う
   * R その場で休む（HP・MPの回復）
   * o ドアや箱を開ける
   * s トラップやドアを探索する
   * k/Ctrl+d アイテムを壊す
* [魔法]
   * m 魔法・祈りを使う
   * G 魔法・祈りを覚える
   * U/O 特殊能力を使う
   * b/P 持っている本を読む（自分の使える魔法の効力を調べる）
* [装備] 
   * w 武器・防具などを装備する
   * t 武器・防具などの装備を外す
   * F ランタンに油を補充する
* [確認] 
   * x 店で売っている物の能力を調べる
   * C キャラクターの情報を調べる
* I *鑑定*をした物の能力を見る　
   * i 持ち物の一覧表示
   * l/x 周りを見る
   * M 現在の階の全体マップを見る
   * ~ 様々な重要な情報を見るメニューを開く
* [その他]
   * p ペットに命令する
   * = オプション設定
   * $ 自動拾いファイルを再読み込みする
   * _ 自動拾いファイルの編集
   * n/[  前のコマンドを繰り返す
   * @  マクロの設定をする
* [ヘルプ]
   * ? オンライン・ヘルプを開く ( a-z で項目選択。スペースで次ページ。)


## アイテム表記

* (XdY)
   * 打撃武器に使われる表記でその武器の基本攻撃力を示します。
   * Y面ダイスをX個振った合計のダメージを与えられます。

* (+h,+d)
   * 基本的に武器に使われる表記で、攻撃力が増加します。
   * +hは命中率、+dはダメージに対する修正を示します。
   * 武器以外に攻撃力が増加する鎧や指輪、アミュレットで使われる場合もあります。
   * 重い鎧では+dの部分が省略されて(-h)とのみ表示され、鎧の重さのせいで動きが鈍り、命中率が下がることを示します。

* \[a,+b\]([+b])
   * 防具なら[a,+b]、それ以外なら[+b]と表示されます。
   * aは防具本来の防御力(AC,アーマークラス)を示し、+bは魔法による増強を示します。

* (+p)
   * そのアイテムが独自に持つ効果の修正を示します。
   * ダメージの指輪ならダメージ修正が+pされ、スピードの指輪なら+pの速度を得ることができます。
   * 武器や防具の場合は(+p隠密)や(+p探索)といったふうに効果が分かるように書かれる場合もあります(その場合でも全ての効果のうち1つしか分かりません)。

* (xn)
   * 飛び道具に使われる表記で、その飛び道具のダメージ倍率を示します。
   * このnの値に腕力修正を加えたものが射撃武器倍率になり(キャラクターの情報コマンド'C'で確認可能)、大きいほど矢や弾を強く撃つことができます。

> 例1)
> (聖戦者)ダガー (2d4) (+10,+15) [+2] (+3)

  このダガーは基本ダメージが4面ダイス2つで命中率に+10,ダメージに+15のボーナスがあります。さらに、防御力が2上昇し、+3の修正を何かに得ることができます(何が+3されるかは*鑑定*することによって分かります)。

> 例2)
> エルフの鎖かたびら (-2) [14,+12] (+3隠密)

この鎖かたびらはその重量のせいで命中率が2下がります。
基本防御力が14、魔法による防御力が12で合計26のACを得ることができます。
さらに隠密行動能力に+3の修正を得られます。

アイテムの頭に★や☆がついているものはアーティファクトと呼ばれるもので、世界に1つしか存在しません。
★がついたものは最初から用意されているもので、発見したものは知識コマンド'~'の既知の伝説のアイテムで見ることができます。

全ての★や☆がついたアイテムや、多くの名のあるアイテムのように見ただけでは効果が分からないものは、*鑑定*することによってそのアイテムの能力を完全に知ることができます。

## 地形

自分のいる場所の地形によって様々な影響を受けます。周りを見るコマンド('l'又は'x')でそこの地形が何かを調べることができます。

* '.': 床, 地面
   * 普通に通過することのできる床や地面です。

* '#': 壁, 木, 草, 暗い穴
   * Windowsでは壁は灰色の■で表されます。
   * 壁は普通は通り抜けられません。
   * 木はレンジャーや浮遊能力のあるときは普通に通り抜けられますが、それ以外では通常よりもターン数がかかります。
   * 草は普通に通り抜けられます。
   * 暗い穴は浮遊がないと通れません。

* '~': 水, 溶岩
   * 青の'\~'は深い水, 薄い青の'\~'は浅い水,赤の'\~'は深い溶岩、薄い赤の'\~'は浅い溶岩です。
   * アイテムを持ちすぎて重量が多いと深い水/溶岩で溺れてダメージを受けます。
   * 溶岩の上を通ると火のダメージを受けます。

* '^': 山, トラップ
   * 茶色の'^'は山で、地上で浮遊能力があれば通り抜けられます。
   * それ以外の状況では通り抜けられません。
   * 山以外の'^'はトラップでさまざまな悪い効果のある罠が仕掛けられています。

* '<', '>': 階段, 坑道
   * '<'は上りの、'>'は下りです。
   * 白は階段で階を1つ移動し、茶色は坑道で階を2つ移動します。


# ちょっと高度なプレイ

  知らなくてもプレイはできますが、知っているとより快適なプレイが楽しめます。

## マクロ

  変愚蛮怒ではある一連の動作を1つのキーに割り当てることができます。
  例えば、a)の位置に[カオスの徴]を持っているときにマジックミサイルの魔法を
  使うとき、普通のコマンド入力ではまず'm'で魔法の使用を選択、'a'で
  魔法書[カオスの徴]を選択、'a'でマジックミサイルを選択、というふうになります。
  つまり、'maa'と3回キーを押さなければなりません。
  マクロを使えばこの動作を1つのキーを押すだけで実現できます。

  例えば、次のようにすればF1キーを押すとマジックミサイルを撃つマクロを作る
  ことができます。
  まず、'@'を押してマクロの設定に入ります。
  そこで「(4) マクロの作成」を選び、「トリガーキー:」のところでF1を押します。
  そして「マクロ行動:」のところで maa と入力し、リターンを押します。
  これでF1キーを押すだけで maa と入力するのと同じ意味になります。

  作ったマクロはセーブしないと、次にゲームを起動したときにまた登録しなおす
  必要があります。
  「(2) ファイルにマクロを追加」を選べばセーブできます。
  このとき、ファイル名を「(名前).prf」にしておくと名前が(名前)のキャラで
  プレイするときに自動的に読み込まれます。

## 銘

  変愚蛮怒ではアイテムに対して'{'コマンドで「銘」をつけることができます。
  ただ好きな名前を付けるだけでなく、一部の文字には特別な効果があります。
  どの文字にどういう効果があるかについて、次に解説します。
  なお、文中のコマンドはオリジナルキー配置でのコマンドを示します。
  うまく銘を付けて上記のマクロと組み合わせると、非常に快適にプレイできます。

* '=g'
   * この銘が書かれたアイテムは自動的に拾います。
   * 矢に書いておくと便利でしょう。

* '#'
   * アイテムを'#'以降に書かれた名前のアーティファクトであるかのように表示します。
   * 例えば、ダガーに'#チキンナイフ'という銘をつけると、「ダガー『チキンナイフ』」と表示されます。
   * 表示が変わるだけでアイテムの持つ効能は一切変わりません。

* '@a?'(aは任意のコマンド文字, ?は任意のタグ文字)
   * アイテムがどの位置にあっても固定した文字でそのアイテムを指定できるようにする為の銘です。コマンド'a'でアイテムを選択するときに、文字キー'?'を押すことによってこの銘の付いたアイテムを選択できます。
   * 魔法を使うときは'@ma'等のアルファベットを使った銘が便利です。

* '@?'(?は0～9のタグ数字)
   * 任意のコマンドにおいてアイテムを選択するときに、数字キー'?'を押すことによってこの銘の付いたアイテムを選択できます。
   * この形式の場合はアルファベットは使用できません。

* '!a'(aは任意のコマンド)
   * そのアイテムを'a'で実行できるコマンドで選択したときに確認をとります。
   * 例えば、帰還の詔の巻物に'!r'とつけておけば、巻物を読むコマンド'r'でその帰還の詔の巻物を選択したときに本当に使うか質問されるため、うっかり読んでしまうことを防止できます。
   *  '!*'という銘を付けた場合は、あらゆるコマンドで確認をとります。

* '.'
   * 一部のアイテムにはランダムなテレポートを引き起こすものがあり、そのアイテムを装備するとたまに「テレポートしますか？[Y/N]」という確認メッセージが出てプレイが中断されますが、ほとんどの場合はテレポートする必要はありません。
   * '.'という銘をつけておけば、この確認メッセージを表示しなくなります。

* '$'
   * 一部のアイテムには警告の能力があり, トラップにかかりそうな時や, 強い攻撃をする敵の視界に入るときに警告しますが, '$'という銘をつけておけば, この警告がなくなります. この銘の使用は推奨しません.

* '^a'(aは任意のコマンド)
   * この銘は装備しているアイテムにつけると効果があります。
   * この銘がついたアイテムを装備すると、コマンド'a'を使うときに確認があります。
   * 例えば、'^k'と銘をつけたアイテムを装備していると、アイテム破壊コマンド'k'を使うときには必ず確認されます。

* '%'/'%all'/'%%'/'%%all'
   * これらの文字列を'{'で刻むと、武器、防具の耐性や能力を表わす文字に自動的に置き換えられます。'%' と刻むと漢字、'%%'ならアルファベットです。
   * 例えば火の耐性なら 'r火' または 'rFi'等です。
   * ただし、既に耐性が全て判っている(*鑑定*されている)アイテムに限ります。
   * 'all'が付いていない時は固定アーティファクとやエゴアイテムのデフォルトの耐性は無視して、追加耐性のみを表示します。'all'が付いていれば全て表示します。
   * この機能に慣れると、いちいち 'I'コマンドで耐性を表示する必要がなく一目で追加耐性が判るので装備を選ぶ時に便利でしょう。

  これらのコマンドは複合することができます。
  '@w0@t1!k!k!d#ほげ' という銘をつけると、装備するときには0で、装備から外す
  ときには1で選択でき、破壊しようとするときには2回、床に落とそうとするときには
  1回確認があり、名前が『ほげ』であるアイテムとなります。
  アイテムの名前に変な文字が入らないよう、'#'を付けるときは最後につけることを
  お勧めします。

## 自動拾い/自動破壊

* 簡単な自動拾い
   * 銘に'=g'が含まれているアイテムは自動的に拾います。
   * 矢等にこういう銘をつけると便利でしょう。また、オプション設定の(1)にある、「常にアイテムを拾う」をはいにすると、全てのアイテムを自動的に拾います。

* 簡単な自動破壊
   * オプション設定の(6)にある、「アイテムの自動破壊を使用する」をはいにすると、アイテムを自動的に破壊します。
   * このままでは全てのアイテムを破壊しようとするので、同じくオプションの6番の、「価値があるアイテムは壊さない」～「がらくたは壊さない」をはいにすれば、自動破壊を使用していても対応するアイテムは破壊しなくなります。

* 複雑な自動拾い/自動破壊
   * 細かく自動拾いや自動破壊を設定することもできます。プレイ中に'_'キーを押すと設定ファイルの編集画面に入ります。詳しい設定の書式についてはautopick.txtを参照して下さい。


## タイルグラフィック、二倍幅タイル表示、大画面表示

* グラフィック
   * WindowsとMacではメニューから選んで選択、Linux では起動時に -g オプションを付けると、画面をタイル・グラフィックを使った表示にする事ができます。8x8サイズの旧タイルと16x16サイズのAdam boltタイルがあります。
   * WindowsとMacではメニューから選ぶだけで使えますが、ソースアーカイブには16x16タイルは付属していないので、Linuxの場合はheng_graf.tar.gzを別にダウンロードして内容をlib/xtra/graf/以下に解凍してください。
   * -- -a オプションで使用できます。

* 二倍幅タイル (Big tile)
   * タイルが小さすぎて見難いと感じる場合はさらに「タイルを2倍幅で表示」を選択(Linux では -- -b オプション)すると漢字と同じサイズの正方形のタイルで表示されます。

* 大画面表示 (Big screen)
   * Windows、mac、Linuxのどれでも、プレイ中にマウスでウィンドウの大きさを変えるとマップ画面の表示領域が大きくなり、より広い領域を同時に見渡す事ができます。

# おまけ

  ゲームには直接関係ないおまけの機能です。

## プレイ記録

  UNIXでは ~/.angband/Hengband/ 、それ以外のOSでは変愚蛮怒の中の lib/user/ の
  ディレクトリに playrecord-(名前).txt というファイル名でプレイ記録があります。
  ゲーム中に'|'コマンドで見ることができます。
  どういう行動をしたときに記録をするかは'='コマンドのオプション設定で、
  「(R) プレイ記録オプション」を選択すれば設定できます。


## スコア登録

  死亡したときや引退したときに、インターネットを通じてスコアを登録することが
  できます。
  イントラネット内等、直接外部にアクセスできない人は lib/pref/proxy.prf に
  プロキシのアドレスを書くことによって、プロキシ経由で登録することができます。
  ただし、データを改造したものや、セーブファイルをバックアップしてプレイした
  ものは登録しないで下さい。


--------
> Mr.hoge
> http://hengband.sourceforge.jp/
> echizen＠users.sourceforge.jp
