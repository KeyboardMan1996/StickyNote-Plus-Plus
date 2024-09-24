#  如何增加一个自定义的StickyNote

### NGui::StickyNotePool

用于创建NGui::StickyNote对象，所有的便笺Gui对象都在这个类中创建。

新增类型时需要为这个类新添加一个创建函数。

* 建议修改为注册制，修改之后后续新增类型无需再修改这个类型的函数。

### NF::StickyNote

这个类型用于存放便笺的序列化之后的内容。

每新增一个类型都需要使用这个类派生出一个类来对新的便笺进行存储。

```c++
		//从xml中载入数据		
		virtual void loadNode(xml_node& node) override;
		//存储数据到xml
		virtual void toNode(xml_node& node) override;
		//返回类型
		static std::string getStickyNoteType();
```

主要需要实现的三个方法

### NGui::StickyNote

便笺窗口类，该类定义了所有便笺的基本形态，如上下的工具栏，中间的内容窗口，便笺的移动与隐藏等等。

要实现一个新的便笺，需要通过这个类派生出一个新类型的便笺窗口类。



### NGui::StickyNoteContent

这个类定义了主要的便笺窗口需要哪些接口，他不是一个widget，只是一个接口类。

但是真正的便笺内容窗口需要通过这个类派生出来。

```c++
		//从html中载入内容
		virtual  void loadNote(const std::shared_ptr<NF::StickyNote> note) override;
		//保存内容到html
		virtual  void saveNote(const std::shared_ptr<NF::StickyNote> note) override;
		//链接与便笺窗口的信号和槽
		virtual  void connectStickyNote(StickyNote* stickyNote) override;
		//取消便笺窗口的信号的槽
		virtual	 void disConnectStickyNote(StickyNote* stickyNote) override;
		//判断便笺内容是否为空
		virtual  bool isEmpty() override;
		//初始化页面，可以在里面进行一些对便笺窗口的设置
		virtual  void initGui(StickyNote* stickyNote) override;
```



