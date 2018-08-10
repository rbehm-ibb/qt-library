	QHeaderView *header = horizontalHeader();
	for (int i = 0; i < 5; ++i)
	{
		header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
	}
	header->setStretchLastSection(true);
