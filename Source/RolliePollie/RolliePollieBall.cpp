// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RolliePollie.h"
#include "RolliePollieBall.h"

ARolliePollieBall::ARolliePollieBall()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

    // Create mesh component for the ball
    Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
    Ball->SetStaticMesh(BallMesh.Object);
    Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
    Ball->SetSimulatePhysics(true);
    Ball->SetAngularDamping(0.1f);
    Ball->SetLinearDamping(0.1f);
    Ball->BodyInstance.MassScale = 3.5f;
    Ball->BodyInstance.MaxAngularVelocity = 0;
    Ball->SetNotifyRigidBodyCollision(true);
    RootComponent = Ball;

    // Create a camera boom attached to the root (ball)
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->bDoCollisionTest = false;
    SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
    SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
    SpringArm->TargetArmLength = 1200.f;
    SpringArm->bEnableCameraLag = false;
    SpringArm->CameraLagSpeed = 3.f;

    // Create a camera and attach to boom
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

    // Set up forces
    RollTorque = 50000000.0f;
    JumpImpulse = 350000.0f;
    bCanJump = true; // Start being able to jump
}


void ARolliePollieBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // set up gameplay key bindings
    PlayerInputComponent->BindAxis("MoveRight", this, &ARolliePollieBall::MoveRight);
    PlayerInputComponent->BindAxis("MoveForward", this, &ARolliePollieBall::MoveForward);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARolliePollieBall::Jump);
}

void ARolliePollieBall::MoveRight(float Val)
{
    SpringArm->RelativeRotation.Add(0, Val, 0);
}

void ARolliePollieBall::MoveForward(float Val)
{
    // const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
    // Ball->AddTorque(Torque);

    Ball->AddTorque(Val * RollTorque * FVector::CrossProduct(FVector::UpVector,
							     SpringArm->RelativeRotation.Vector()).GetUnsafeNormal());
    
}

void ARolliePollieBall::Jump()
{
    if(bCanJump)
    {
	const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
	Ball->AddImpulse(Impulse);
	bCanJump = false;
    }
}

void ARolliePollieBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    bCanJump = true;
}
