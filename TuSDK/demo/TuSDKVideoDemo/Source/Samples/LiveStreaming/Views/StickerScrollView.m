//
//  StickerScrollView.m
//  TuSDKVideoDemo
//
//  Created by tutu on 2017/3/10.
//  Copyright © 2017年 TuSDK. All rights reserved.
//

#import "StickerScrollView.h"


@interface StickerScrollView ()<UICollectionViewDelegate,UICollectionViewDataSource>{
    //数据源；
    //贴纸组的数组
    NSArray<TuSDKPFStickerGroup *> *_stickerGroups;
}

@end

@implementation StickerScrollView

#pragma mark - 视图布局方法；
- (instancetype)initWithFrame:(CGRect)frame{
    if ([super initWithFrame:frame]) {
        [self createStickerCollection];
    }
    return self;
}

- (void)createStickerCollection{
    
    //创建FlowLayout方式
    UICollectionViewFlowLayout *layout = [UICollectionViewFlowLayout new];
    layout.itemSize = CGSizeMake(60, 60);
    layout.minimumLineSpacing = 13;
    layout.minimumInteritemSpacing = 13;
    layout.sectionInset = UIEdgeInsetsMake(13, 13, 13, 13);
    layout.scrollDirection = UICollectionViewScrollDirectionVertical;
    
    //创建collection；
    UICollectionView * collectionView = [[UICollectionView alloc]initWithFrame:self.bounds collectionViewLayout:layout];
    collectionView.backgroundColor = self.backgroundColor;
    collectionView.showsVerticalScrollIndicator = false;
    collectionView.showsHorizontalScrollIndicator = false;
    
    [collectionView registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"cell"];
    collectionView.delegate = self;
    collectionView.dataSource = self;
    [self addSubview:collectionView];
    
    //获取贴纸组数据源；
    _stickerGroups = [[TuSDKPFStickerLocalPackage package] getSmartStickerGroups];
    
}

#pragma mark -- collection代理方法 collectionDelegate
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    NSLog(@"选择贴图：%ld", (long)indexPath.row);
    if ([self.stickerDelegate respondsToSelector:@selector(clickStickerViewWith:)]) {
        if (indexPath.row == 0) {
            [self.stickerDelegate clickStickerViewWith:nil];
        }else{
            [self.stickerDelegate clickStickerViewWith:_stickerGroups[indexPath.row-1]];
        }
    }
    
    //给cell添加选中边框；
    if (indexPath.row != 0) {
        UICollectionViewCell *cell = [collectionView cellForItemAtIndexPath:indexPath];
        for (UIView *view in cell.subviews) {
            if (view.tag == 201) {
                view.layer.borderWidth = 2;
                view.layer.borderColor = lsqRGB(244, 161, 24).CGColor;
            }
        }

    }
}

- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row != 0) {
        //给上一个选中的cell取消选中边框；
        UICollectionViewCell *cell = [collectionView cellForItemAtIndexPath:indexPath];
        for (UIView *view in cell.subviews) {
            if (view.tag == 201) {
                view.layer.borderWidth = 2;
                view.layer.borderColor = [UIColor clearColor].CGColor;
            }
        }
    }

}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return _stickerGroups.count+1;
}

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView{
    return 1;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    
    //设置cell
    UICollectionViewCell * cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"cell" forIndexPath:indexPath];
    for (UIView *view in cell.subviews) {
        [view removeFromSuperview];
    }
    
    CGRect rect;
    if (indexPath.row != 0) {
        rect = CGRectMake(0, 0, cell.lsqGetSizeWidth, cell.lsqGetSizeHeight);
    }else{
        rect = CGRectMake(0, 0, 40, 40);
    }
    UIImageView *iv = [[UIImageView alloc]initWithFrame:rect];
    iv.center = CGPointMake(cell.lsqGetSizeWidth/2, cell.lsqGetSizeHeight/2);
    iv.contentMode = UIViewContentModeScaleAspectFit;
    iv.image = nil;
    //注意：201为查找该cell上的iv时用的标志tag;
    iv.tag = 201;
    iv.contentMode = UIViewContentModeScaleToFill;
    if (indexPath.row == 0) {
        //第一张图固定；
        iv.image = [UIImage imageNamed:@"video_style_default_btn_sticker_off"];
    }else{
        //获取对应贴纸的缩略图；
        [[TuSDKPFStickerLocalPackage package] loadThumbWithStickerGroup:_stickerGroups[indexPath.row-1] imageView:iv];
    }
    
    [cell addSubview:iv];
    
    return cell;
}


@end
