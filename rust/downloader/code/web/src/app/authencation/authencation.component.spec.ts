import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AuthencationComponent } from './authencation.component';

describe('AuthencationComponent', () => {
  let component: AuthencationComponent;
  let fixture: ComponentFixture<AuthencationComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AuthencationComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AuthencationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
