//
//  KSYFilterUnityView.m
//
//  Created by 孙健 on 16/6/24.
//  Copyright © 2016年 ksyun. All rights reserved.
//
#import <GPUImage/GPUImage.h>
#import "KSYFaceUnityView.h"
#import "KSYNameSlider.h"
#import "KSYFaceUnityWrapper.h"

@interface KSYFaceUnityView()<UICollectionViewDelegate, UICollectionViewDataSource> {
    NSMutableArray  *resourceArray;

    // 滤镜
    UILabel * lblSeg;
    UISegmentedControl  *filterSeg;
    
    // 美颜参数调节
    KSYNameSlider *cheekThinningSlider;              //瘦脸
    KSYNameSlider *eyeEnlargingSlider;                 //大眼
    KSYNameSlider *colorLevelSlider;                    //美白
    KSYNameSlider *blurLeveSlider;                       //磨皮
    
    //贴纸Collect
    UICollectionView *stickerCollect;
}

@end

@implementation KSYFaceUnityView

- (id)init{
    self = [super init];
    
    lblSeg = [self addLable:@"滤镜"];
    filterSeg = [self addSegCtrlWithItems:
                        @[ @"nature",
                           @"delta",
                           @"electric",
                           @"slowlived",
                           @"tokyo",
                           @"warm",
                           ]];
    filterSeg.selectedSegmentIndex = 0;
    
    cheekThinningSlider = [self addSliderName:@"瘦脸" From:0.0 To:2.0 Init:1.0];
    eyeEnlargingSlider = [self addSliderName:@"大眼" From:0.0 To:2.0 Init:1.0];
    colorLevelSlider = [self addSliderName:@"美白" From:0.0 To:2.0 Init:1.0];
    blurLeveSlider = [self addSliderName:@"磨皮" From:0.0 To:6.0 Init:5.0];
    
    //创建collectionview通过一个布局策略来创建
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
    //设置布局方向为垂直流布局
    layout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
    layout.itemSize = CGSizeMake(50, 30);
    stickerCollect = [[UICollectionView alloc] initWithFrame:CGRectMake(0, 0, self.width, 60)  collectionViewLayout:layout];
    //设置代理
    stickerCollect.delegate = self;
    stickerCollect.dataSource = self;
    stickerCollect.backgroundColor = [UIColor blackColor];
    stickerCollect.alpha = 0.5;
    //组册item类型，这里使用系统的类型
    [stickerCollect registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"cellid"];
    
    resourceArray = [NSMutableArray arrayWithArray:
                     @[@"关闭",
                       @"tiara",
                       @"YellowEar",
                       @"tears",
                       @"PrincessCrown",
                       @"Mood",
                       @"Deer",
                       @"BeagleDog",
                       @"HappyRabbi",
                       @"hartshorn",
                       @"item0204",
                       @"item0208",
                       @"item0210",
                       @"item0501",
                       ]];
    
    NSMutableArray *item_names = [NSMutableArray array];
    
    [resourceArray enumerateObjectsUsingBlock:^(NSString *obj, NSUInteger idx, BOOL * _Nonnull stop) {
        [item_names addObject:[obj stringByAppendingString:@".bundle"]];
    }];

    _stickerFilter  = [[KSYFaceUnityWrapper alloc]initWithArray:item_names];
    
    return self;
}

- (void)layoutUI{
    [super layoutUI];
    self.yPos = 0;
    
    [self putLable:lblSeg andView: filterSeg];
    [self putRow1:cheekThinningSlider];
    [self putRow1:eyeEnlargingSlider];
    [self putRow1:colorLevelSlider];
    [self putRow1:blurLeveSlider];
    
    stickerCollect.frame = CGRectMake(0,  CGRectGetMaxY(blurLeveSlider.frame), self.width, 60);
    [self addSubview:stickerCollect];
}

#pragma mark - sticker collection
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    UICollectionViewCell *cell = [collectionView  cellForItemAtIndexPath:indexPath];
    cell.backgroundColor = [UIColor blueColor];
    
    _stickerFilter.stickerIndex = (int)indexPath.row;
}

- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath{
    UICollectionViewCell *cell = [collectionView  cellForItemAtIndexPath:indexPath];
    cell.backgroundColor = [UIColor whiteColor];
}

//返回分区个数
-(NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView{
    return 1;
}

//返回每个分区的item个数
-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return resourceArray.count;
}

//获取cell
-(UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    UICollectionViewCell * cell  = [collectionView dequeueReusableCellWithReuseIdentifier:@"cellid" forIndexPath:indexPath];
    cell.backgroundColor = [UIColor whiteColor];
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 50, 30)];
    label.textColor = [UIColor redColor];
    label.textAlignment =  NSTextAlignmentCenter;
    label.font = [UIFont fontWithName:@"Courier New" size:12.0f];
    label.text = [NSString stringWithFormat:@"%@",[resourceArray objectAtIndex:indexPath.row]];
    for (id subView in cell.contentView.subviews) {
        [subView removeFromSuperview];
    }
    [cell.contentView addSubview:label];
    return cell;
}

#pragma mark - beautify slider
- (IBAction)onSlider:(id)sender {
    if(sender == cheekThinningSlider)
        _stickerFilter.cheekThinning = cheekThinningSlider.value;
    else if(sender == eyeEnlargingSlider)
        _stickerFilter.eyeEnlarging = eyeEnlargingSlider.value;
    else if(sender == colorLevelSlider)
        _stickerFilter.colorLevel = colorLevelSlider.value;
    else if(sender == blurLeveSlider)
        _stickerFilter.blurLevel = blurLeveSlider.value;
    [super onSlider:sender];
}

#pragma mark - filter Segment
- (IBAction)onSegCtrl:(id)sender {
    _stickerFilter.filterIndex = (int)(filterSeg.selectedSegmentIndex);
}
@end
